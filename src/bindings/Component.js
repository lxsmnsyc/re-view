'use strict';

function make(name, render) {
  return {
    name: name,
    render: render,
  };
}

exports.make = make;