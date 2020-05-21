function makeMap() {
  return new Map();
}

function mapSet(map, key, value) {
  map.set(key, value);
}

function mapGet(map, key) {
  return map.get(key);
}

function mapHas(map, key) {
  return map.has(key);
}

module.exports.makeMap = makeMap;
module.exports.mapSet = mapSet;
module.exports.mapGet = mapGet;
module.exports.mapHas = mapHas;
