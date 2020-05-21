function makeArray() {
  return [];
}

function arraySet(array, key, value) {
  array[key] = value;
}

function arrayGet(array, key) {
  return array[key];
}

function arrayForEach(array, callback) {
  return array.forEach(callback);
}

module.exports.makeArray = makeArray;
module.exports.arraySet = arraySet;
module.exports.arrayGet = arrayGet;
module.exports.arrayForEach = arrayForEach;
