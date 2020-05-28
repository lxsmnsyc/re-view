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

const FIBER = 'fiberInstance';

const CUSTOM_ATTR = 're-view-index';
const CUSTOM_PROP = 'reviewIndex';

const ID_ATTR = 're-view-id';
const ID_PROP = 'reviewID';

function createInstance(constructor, props, id, fiber) {
  if (constructor === 'text') {
    const node = document.createTextNode(props.value);
    node[ID_PROP] = id;
    node[FIBER] = fiber;
    return node;
  }

  const node = document.createElement(constructor);
  node.setAttribute(ID_ATTR, id);
  node[ID_PROP] = id;
  node[FIBER] = fiber;

  Object.entries(props).forEach(([key, value]) => {
    if (key === 'class') {
      node.class = value;
    } else {
      node.setAttribute(key, value);
    }
  });

  return node;
}

function appendChild(parentInstance, childInstance, index, fiber) {
  childInstance[FIBER] = fiber;

  childInstance[CUSTOM_PROP] = index.toString();
  if (childInstance instanceof Element) {
    childInstance.setAttribute(CUSTOM_ATTR, index.toString());
  }

  let currentChild = parentInstance.firstChild;

  while (currentChild) {
    let actualIndex = Number.parseInt(currentChild[CUSTOM_PROP], 10);

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
    currentChild = currentChild.nextSibling;
  }

  parentInstance.appendChild(childInstance);
}

function removeChild(parentInstance, childInstance, index, fiber) {
  childInstance[FIBER] = fiber;

  let currentChild = parentInstance.firstChild;

  while (currentChild) {
    let actualIndex = Number.parseInt(currentChild[CUSTOM_PROP], 10);

    if (actualIndex === index) {
      parentInstance.removeChild(currentChild);
    }
    currentChild = currentChild.nextSibling;
  }
}

function commitUpdate(instance, oldProps, newProps, index, fiber) {
  instance[FIBER] = fiber;
  instance[instance] = index.toString();
  if (instance instanceof Element) {
    instance.setAttribute(CUSTOM_ATTR, index.toString());
  } else {
    instance.nodeValue = newProps.value;
    return;
  }

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
