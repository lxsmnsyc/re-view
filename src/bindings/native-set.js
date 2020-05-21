function makeSet() {
  return new Set();
}

function setAdd(set, value) {
  set.add(value);
}

function setHas(set, value) {
  return set.has(value);
}

function setForEach(set, forEach) {
  return set.forEach(forEach);
}

module.exports.makeSet = makeSet;
module.exports.setAdd = setAdd;
module.exports.setHas = setHas;
module.exports.setForEach = setForEach;
