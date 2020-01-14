'use strict';

const contextStack = [];
let active = false;
let current;
let level = 0;

function setContext(value) {
  contextStack[level++] = value;
  active = true;
  current = value;
}

function getContext() {
  return current;
}

function isActive() {
  return active;
}

function popContext() {
  current = contextStack[--level];

  if (level === 0) {
    active = false;
  }
}

exports.setContext = setContext;
exports.getContext = getContext;
exports.popContext = popContext;
exports.isActive = isActive;