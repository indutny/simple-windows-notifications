#include <winrt/windows.data.xml.dom.h>
#include <winrt/windows.foundation.metadata.h>
#include <winrt/windows.ui.notifications.h>
#include <windows.h>
#include <string.h>

#include "napi.h"

using namespace winrt;
using namespace Windows::UI::Notifications;
using namespace Windows::Data::Xml::Dom;

static bool are_notifications_supported =
  Windows::Foundation::Metadata::ApiInformation::IsApiContractPresent(
    L"Windows.Foundation.UniversalApiContract", 65536);
static bool is_expires_on_reboot_supported =
  Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(
    L"Windows.UI.Notifications.ToastNotification", L"ExpiresOnReboot");

void EnsureNotificationsSupported(Napi::Env env) {
  if (!are_notifications_supported) {
    throw Napi::Error::New(env, "Unsupported on current OS");
  }
}

Napi::Error FromWinRTError(Napi::Env env, const winrt::hresult_error& ex) {
  static char buf[1024];
  auto code = static_cast<int>(ex.code());
  auto message = ex.message();

  snprintf(
    buf,
    sizeof(buf),
    "WinRT exception code: %d, msg: %s", code, message.c_str());
  throw Napi::Error::New(env, buf);
}

void ShowNotification(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 5) {
    throw Napi::Error::New(env, "Invalid argument count");
  }

  auto appId = info[0].As<Napi::String>();
  auto toastXml = info[1].As<Napi::String>();
  auto tag = info[2].As<Napi::String>();
  auto group = info[3].As<Napi::String>();
  auto expiresOnReboot = info[4].As<Napi::Boolean>();
  if (!appId.IsString() || !toastXml.IsString() ||
      !tag.IsString() || !group.IsString() || !expiresOnReboot.IsBoolean()) {
    throw Napi::Error::New(env, "Invalid argument type");
  }

  EnsureNotificationsSupported(env);

  try {
    XmlDocument xml;
    xml.LoadXml(to_hstring(toastXml.Utf8Value()));
    ToastNotification notification(xml);
    notification.Tag(to_hstring(tag.Utf8Value()));
    notification.Group(to_hstring(group.Utf8Value()));
    if (is_expires_on_reboot_supported) {
      notification.ExpiresOnReboot(expiresOnReboot.Value());
    }

    auto notifier = ToastNotificationManager::CreateToastNotifier(
        to_hstring(appId.Utf8Value()));
    notifier.Show(notification);
  } catch (winrt::hresult_error const& ex) {
    throw FromWinRTError(env, ex);
  }
}


void RemoveNotification(const Napi::CallbackInfo& info) {
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

  EnsureNotificationsSupported(env);

  try {
    ToastNotificationManager::History().Remove(
        to_hstring(tag.Utf8Value()),
        to_hstring(group.Utf8Value()),
        to_hstring(appId.Utf8Value()));
  } catch (winrt::hresult_error const& ex) {
    throw FromWinRTError(env, ex);
  }
}

void ClearHistory(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() != 1) {
    throw Napi::Error::New(env, "Invalid argument count");
  }

  auto appId = info[0].As<Napi::String>();
  if (!appId.IsString()) {
    throw Napi::Error::New(env, "Invalid argument type");
  }

  EnsureNotificationsSupported(env);

  try {
    ToastNotificationManager::History().Clear(
        to_hstring(appId.Utf8Value()));
  } catch (winrt::hresult_error const& ex) {
    throw FromWinRTError(env, ex);
  }
}

void SendDummyKeystroke(const Napi::CallbackInfo& info) {
  // See https://chromium.googlesource.com/chromium/src.git/+/5c432815bbb22210f7c995bbb508359f64baadf5/chrome/notification_helper/notification_activator.cc#155
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
  exports.Set(Napi::String::New(env, "removeNotification"),
      Napi::Function::New(env, RemoveNotification));
  exports.Set(Napi::String::New(env, "clearHistory"),
      Napi::Function::New(env, ClearHistory));
  exports.Set(Napi::String::New(env, "sendDummyKeystroke"),
      Napi::Function::New(env, SendDummyKeystroke));
  return exports;
}

NODE_API_MODULE(simple-windows-notifications, Init)
