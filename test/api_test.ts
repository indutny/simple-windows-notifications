import test from 'ava';

import { Notifier, sendDummyKeystroke } from '../index';

test('sends dummy keystroke', async (t) => {
  if (process.platform === 'win32') {
    t.notThrows(() => sendDummyKeystroke());
  } else {
    t.throws(() => sendDummyKeystroke(), {
      message: 'This library works only on Windows',
    });
  }
});

test('shows and removes notification', async (t) => {
  const notifier = new Notifier('org.indutny.test');

  const toastXml = `<toast>
    <visual>
      <binding template="ToastText02">
        <text id="1">headlineText</text>
        <text id="2">bodyText</text>
      </binding>
    </visual>
  </toast>`;

  if (process.platform === 'win32') {
    t.notThrows(() => {
      notifier.clearAll();
      notifier.show(toastXml, { tag: 'tag', group: 'group' });
      notifier.remove({ tag: 'tag', group: 'group' });
    });
  } else {
    t.throws(
      () => {
        notifier.clearAll();
        notifier.show(toastXml, { tag: 'tag', group: 'group' });
        notifier.remove({ tag: 'tag', group: 'group' });
      },
      { message: 'This library works only on Windows' },
    );
  }
});
