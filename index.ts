import loadBinding from 'bindings';

type BindingType = Readonly<{
  showNotification(
    appId: string,
    toastXml: string,
    tag: string,
    group: string,
    expiresOnReboot: boolean,
  ): void;
  removeNotification(appId: string, tag: string, group: string): void;
  clearHistory(appId: string): void;
  sendDummyKeystroke(): void;
}>;

let binding: BindingType | undefined;
if (process.platform === 'win32') {
  binding = loadBinding({
    bindings: 'simple-windows-notifications',
    try: [
      [
        'module_root',
        'prebuilds',
        `${process.platform}-${process.arch}`,
        '@indutny+simple-windows-notifications.node',
      ],
      ['module_root', 'build', 'Release', 'bindings'],
      ['module_root', 'build', 'Debug', 'bindings'],
    ],
  });
}

/**
 * Options for showing a notification.
 */
export type ShowNotificationOptionsType = Readonly<{
  /**
   * Notification tag string, used to identify notifications when removing
   * them.
   */
  tag: string;

  /**
   * Notification group string, used to identify notifications when removing
   * them.
   */
  group: string;

  /**
   * If true - the notification will be cleared after reboot.
   */
  expiresOnReboot?: boolean;
}>;

/**
 * Options for removing a notification.
 */
export type RemoveNotificationOptionsType = Readonly<{
  /**
   * Notification tag string, used to identify notifications when removing
   * them.
   */
  tag: string;

  /**
   * Notification group string, used to identify notifications when removing
   * them.
   */
  group: string;
}>;

/**
 * Main class
 */
export class Notifier {
  /**
   * @constructor
   * @param appId - Application id, typically: 'org.nodejs.node'
   */
  constructor(private readonly appId: string) {}

  /**
   * Show a notification with a given toast XML.
   *
   * @param toastXml - See https://learn.microsoft.com/en-us/previous-versions/windows/apps/hh761494(v=win.10)
   * @param options - Notification data use to identify the notification.
   */
  public show(
    toastXml: string,
    { tag, group, expiresOnReboot = false }: ShowNotificationOptionsType,
  ) {
    if (!binding) {
      throw new Error('This library works only on Windows');
    }
    binding.showNotification(this.appId, toastXml, tag, group, expiresOnReboot);
  }

  /**
   * Remove a notification with a given tag/group
   *
   * @param toastXml - See https://learn.microsoft.com/en-us/previous-versions/windows/apps/hh761494(v=win.10)
   * @param options - Notification data use to identify the notification.
   */
  public remove({ tag, group }: RemoveNotificationOptionsType): void {
    if (!binding) {
      throw new Error('This library works only on Windows');
    }
    binding.removeNotification(this.appId, tag, group);
  }

  /**
   * Remove all notifications sent by this app.
   */
  public clearAll(): void {
    if (!binding) {
      throw new Error('This library works only on Windows');
    }
    binding.clearHistory(this.appId);
  }
}

/**
 * Send a dummy keystroke to the app. Needed when starting a second instance
 * of the app from the notification manager and bringing the first instance to
 * the foreground.
 *
 * See: https://chromium.googlesource.com/chromium/src.git/+/5c432815bbb22210f7c995bbb508359f64baadf5/chrome/notification_helper/notification_activator.cc#155
 * See: https://www.npmjs.com/package/windows-dummy-keystroke#but-why
 */
export function sendDummyKeystroke() {
  if (!binding) {
    throw new Error('This library works only on Windows');
  }
  binding.sendDummyKeystroke();
}
