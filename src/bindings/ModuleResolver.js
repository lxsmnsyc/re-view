'use script';

const modules = new Map();

function setModule(key, module) {
  modules.set(key, module);
}

function getModule(key) {
  return modules.get(key);
}

exports.setModule = setModule;
exports.getModule = getModule;