#include <winrt/windows.ui.notifications.h>
#include <winrt/windows.data.xml.dom.h>
#include <windows.h>

#include "napi.h"

using namespace winrt;
using namespace Windows::UI::Notifications;
using namespace Windows::Data::Xml::Dom;

void ShowNotification(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 4) {
    throw Napi::Error::New(env, "Invalid argument count");
  }

  auto appId = info[0].As<Napi::String>();
  auto toastXml = info[1].As<Napi::String>();
  auto tag = info[2].As<Napi::String>();
  auto group = info[3].As<Napi::String>();
  if (!appId.IsString() || !toastXml.IsString() || !tag.IsString() ||
      !group.IsString()) {
    throw Napi::Error::New(env, "Invalid argument type");
  }

  XmlDocument xml;
  xml.LoadXml(to_hstring(toastXml.Utf8Value()));
  ToastNotification notification(xml);
  notification.Tag(to_hstring(tag.Utf8Value()));
  notification.Group(to_hstring(group.Utf8Value()));

  auto notifier = ToastNotificationManager::CreateToastNotifier(
      to_hstring(appId.Utf8Value()));
  notifier.Show(notification);
}


void ClearHistory(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() != 3) {
    throw Napi::Error::New(env, "Invalid argument count");
  }

  auto appId = info[0].As<Napi::String>();
  auto tag = info[1].As<Napi::String>();
  auto group = info[2].As<Napi::String>();
  if (!appId.IsString() || !tag.IsString() || !group.IsString()) {
    throw Napi::Error::New(env, "Invalid argument type");
  }

  ToastNotificationManager::History().Remove(
      to_hstring(tag.Utf8Value()),
      to_hstring(group.Utf8Value()),
      to_hstring(appId.Utf8Value()));
}

void SendDummyKeystroke(const Napi::CallbackInfo& info) {
  INPUT inputs[2] = {};

  inputs[0].type = INPUT_KEYBOARD;
  inputs[0].ki.dwFlags = 0;  // Key down
  inputs[1] = inputs[0];
  inputs[1].ki.dwFlags |= KEYEVENTF_KEYUP;  // Key up

  SendInput(2, inputs, sizeof(inputs[0]));
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "showNotification"),
      Napi::Function::New(env, ShowNotification));
  exports.Set(Napi::String::New(env, "clearHistory"),
      Napi::Function::New(env, ClearHistory));
  exports.Set(Napi::String::New(env, "sendDummyKeystroke"),
      Napi::Function::New(env, SendDummyKeystroke));
  return exports;
}

NODE_API_MODULE(addon, Init)