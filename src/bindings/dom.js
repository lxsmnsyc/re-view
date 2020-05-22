/**
 * @license
 * MIT License
 *
 * Copyright (c) 2020 Alexis Munsayac
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *
 * @author Alexis Munsayac <alexis.munsayac@gmail.com>
 * @copyright Alexis Munsayac 2020
 */
'use strict';

const CUSTOM_ATTR = 're-view-id';
const CUSTOM_PROP = 'reviewID';

function flatten({ aria }) {
  return {
    ...aria,
  };
}

function createInstance(constructor, props) {
  if (constructor === 'text') {
    return document.createTextNode(props.value);
  }
  const node = document.createElement(constructor);

  props = flatten(props);

  Object.entries(props).forEach(([key, value]) => {
    if (key === 'class') {
      node.class = value;
    } else {
      node.setAttribute(key, value);
    }
  });

  return node;
}

function appendChild(parentInstance, childInstance, index) {
  let nodes = parentInstance.children;
  childInstance[CUSTOM_PROP] = index.toString();
  if (childInstance instanceof Element) {
    childInstance.setAttribute(CUSTOM_ATTR, index.toString());
  }

  for (let i = 0; i < nodes.length; i += 1) {
    let currentChild = nodes[i];
    let actualIndex = Number.parseInt(nodes[i][CUSTOM_PROP], 10);

    /**
     * Replace instance if it is of the same value
     */
    if (actualIndex === index) {
      parentInstance.replaceChild(childInstance, currentChild);
      return;
    } else if (index < actualIndex) {
      /**
       * If the the previous index is skipped (prevIndex < index < actualIndex)
       * insert in between.
       */
      parentInstance.insertBefore(childInstance, currentChild);
      return;
    }
  }

  parentInstance.appendChild(childInstance);
}

function removeChild(parentInstance, childInstance, index) {
  let nodes = parentInstance.children;

  for (let i = 0; i < nodes.length; i += 1) {
    let actualIndex = Number.parseInt(nodes[i][CUSTOM_PROP], 10);

    if (actualIndex === index) {
      parentInstance.removeChild(childInstance);
    }
  }
}

function commitUpdate(instance, oldProps, newProps, index) {
  oldProps = flatten(oldProps);
  newProps = flatten(newProps);

  Object.keys(oldProps).forEach((key) => {
    if (newProps[key] == null) {
      if (key === 'class') {
        instance.class = null;
      } else {
        instance.removeAttribute(key);
      }
    }
  });
  Object.entries(newProps).forEach(([key, value]) => {
    if (key === 'class') {
      instance.class = value;
    } else {
      instance.setAttribute(key, value);
    }
  });
}

module.exports.createInstance = createInstance;
module.exports.appendChild = appendChild;
module.exports.removeChild = removeChild;
module.exports.commitUpdate = commitUpdate;
