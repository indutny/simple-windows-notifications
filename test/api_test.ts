import test, { type ExecutionContext } from 'ava';

import { Notifier, sendDummyKeystroke } from '../index';

function platformWrap(t: ExecutionContext, fn: () => void): void {
  if (process.platform === 'win32') {
    t.notThrows(fn);
  } else {
    t.throws(fn, {
      message: 'This library works only on Windows',
    });
  }
}

test('sends dummy keystroke', async (t) => {
  platformWrap(t, () => sendDummyKeystroke());
});

test('shows a notification', async (t) => {
  const notifier = new Notifier('org.indutny.test');

  const toastXml = `<toast>
    <visual>
      <binding template="ToastText02">
        <text id="1">headlineText</text>
        <text id="2">bodyText</text>
      </binding>
    </visual>
  </toast>`;

  platformWrap(t, () => {
    notifier.show(toastXml, { tag: 'tag', group: 'group' });
  });
});

test('shows a notification that expires on reboot', async (t) => {
  const notifier = new Notifier('org.indutny.test');

  const toastXml = `<toast>
    <visual>
      <binding template="ToastText02">
        <text id="1">headlineText</text>
        <text id="2">bodyText</text>
      </binding>
    </visual>
  </toast>`;

  platformWrap(t, () => {
    notifier.show(toastXml, {
      tag: 'tag',
      group: 'group',
      expiresOnReboot: true,
    });
  });
});

test('clears a notification', async (t) => {
  const notifier = new Notifier('org.indutny.test');

  platformWrap(t, () => {
    notifier.remove({ tag: 'tag', group: 'group' });
  });
});

test('clears all notifications', async (t) => {
  const notifier = new Notifier('org.indutny.test');

  platformWrap(t, () => {
    notifier.clearAll();
  });
});
