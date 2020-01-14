'use strict';

function scheduleAsync(fn) {
  Promise.resolve().then(fn);
}

exports.scheduleAsync = scheduleAsync;