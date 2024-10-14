# @indutny/simple-windows-notifications

[![npm](https://img.shields.io/npm/v/@indutny/simple-windows-notifications)](https://www.npmjs.com/package/@indutny/simple-windows-notifications)

[API docs](https://indutny.github.io/simple-windows-notifications).

## Installation

```sh
npm install @indutny/simple-windows-notifications
```

## Usage

```js
import { Notifier } from '@indutny/simple-windows-notifications';

const notifier = new Notifier('org.indutny.test');
notifier.clearAll();

notifier.remove({ tag: 'tag', group: 'group' });

const toastXml = `<toast>
  <visual>
    <binding template="ToastText02">
      <text id="1">headlineText</text>
      <text id="2">bodyText</text>
    </binding>
  </visual>
</toast>`;

notifier.show(toastXml, { tag: 'tag', group: 'group' });
```

See https://learn.microsoft.com/en-us/previous-versions/windows/apps/hh761494(v=win.10)

## LICENSE

This software is licensed under the MIT License.
