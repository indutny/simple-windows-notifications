# @indutny/simple-windows-notifications

[![npm](https://img.shields.io/npm/v/@indutny/simple-windows-notifications)](https://www.npmjs.com/package/@indutny/simple-windows-notifications)
[API docs](https://indutny.github.io/simple-windows-notifications).

## Installation

```sh
npm install @indutny/simple-windows-notifications
```

## Usage

```js
import {
  clearHistory,
  showNotification,
} from '@indutny/simple-windows-notifications';

clearHistory({ appId: 'org.indutny.test', tag: 'tag', group: 'group' });
showNotification(
  `<toast>
    <visual>
      <binding template="ToastText02">
        <text id="1">headlineText</text>
        <text id="2">bodyText</text>
      </binding>
    </visual>
  </toast>`,
  { appId: 'org.indutny.test', tag: 'tag', group: 'group' },
);
```

## Limitations

Because streams are reused between several requests, if one of the requests
gets stalled it might stall all other requests that are based on the same input
stream. Depending on the use case, either more granular contexts could be used
or a timeout could be added to all derived streams.

## LICENSE

This software is licensed under the MIT License.
