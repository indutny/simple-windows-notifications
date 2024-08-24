import loadBinding from 'bindings';

type BindingType = Readonly<{
  clearHistory(appId: string, tag: string, group: string): void;
  showNotification(
    appId: string,
    toastXml: string,
    tag: string,
    group: string,
  ): void;
  sendDummyKeystroke(): void;
}>;

let binding: BindingType | undefined;
if (process.platform === 'win32') {
  binding = loadBinding('simple-windows-notifications');
}

export type NotificationData = Readonly<{
  appId: string;
  tag: string;
  group: string;
}>;

export function clearHistory({ appId, tag, group }: NotificationData): void {
  if (!binding) {
    throw new Error('This library works only on Windows');
  }
  binding.clearHistory(appId, tag, group);
}

export function showNotification(
  toastXml: string,
  { appId, tag, group }: NotificationData,
) {
  if (!binding) {
    throw new Error('This library works only on Windows');
  }
  binding.showNotification(appId, toastXml, tag, group);
}

export function sendDummyKeystroke() {
  if (!binding) {
    throw new Error('This library works only on Windows');
  }
  binding.sendDummyKeystroke();
}
