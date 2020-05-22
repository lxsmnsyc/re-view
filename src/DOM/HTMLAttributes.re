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
[@bs.deriving abstract]
type t = {
  /**
   * Provides a hint for generating a keyboard shortcut for the current element.
   * This attribute consists of a space-separated list of characters. The browser
   * should use the first one that exists on the computer keyboard layout.
   */
  [@bs.as "accesskey"]
  [@bs.optional]
  accessKey: string,

  /**
   * Controls whether and how text input is automatically capitalized as it is
   * entered/edited by the user. It can have the following values:
   * - off or none, no autocapitalization is applied (all letters default to lowercase)
   * - on or sentences, the first letter of each sentence defaults to a capital
   * letter; all other letters default to lowercase
   * - words, the first letter of each word defaults to a capital letter;
   * all other letters default to lowercase
   * - characters, all letters should default to uppercase
   */
  [@bs.as "autocapitalize"]
  [@bs.optional]
  autoCapitalize: string,

  /**
   * A space-separated list of the classes of the element. 
   */
  [@bs.as "class"]
  [@bs.optional]
  className: string,

  /**
   * An enumerated attribute indicating if the element should be editable by
   * the user. If so, the browser modifies its widget to allow editing. The
   * attribute must take one of the following values:
   * - true or the empty string, which indicates that the element must be editable;
   * - false, which indicates that the element must not be editable.
   */
  [@bs.as "contenteditable"]
  [@bs.optional]
  contentEditable: bool,

  /**
   * An enumerated attribute indicating the directionality of the element's text.
   * It can have the following values:
   * - ltr, which means left to right and is to be used for languages that are
   * written from the left to the right (like English);
   * - rtl, which means right to left and is to be used for languages that are
   * written from the right to the left (like Arabic);
   * - auto, which lets the user agent decide. It uses a basic algorithm as it
   * parses the characters inside the element until it finds a character with a
   * strong directionality, then it applies that directionality to the whole element.
   */
  [@bs.as "dir"]
  [@bs.optional]
  dir: string,
  
  /**
   * An enumerated attribute indicating whether the element can be dragged,
   * using the Drag and Drop API. It can have the following values:
   * - true, which indicates that the element may be dragged
   * - false, which indicates that the element may not be dragged.
   */
  [@bs.as "draggable"]
  [@bs.optional]
  draggable: bool,
  
  /**
   * A Boolean attribute indicates that the element is not yet, or is no longer,
   * relevant. For example, it can be used to hide elements of the page that can't
   * be used until the login process has been completed. The browser won't render
   * such elements. This attribute must not be used to hide content that could
   * legitimately be shown.
   */
  [@bs.as "hidden"]
  [@bs.optional]
  hidden: bool,

  /**
   * Defines a unique identifier (ID) which must be unique in the whole document.
   * Its purpose is to identify the element when linking (using a fragment identifier),
   * scripting, or styling (with CSS).
   */
  [@bs.as "id"]
  [@bs.optional]
  id: string,

  /**
   * Provides a hint to browsers as to the type of virtual keyboard configuration to use
   * when editing this element or its contents. Used primarily on <input> elements, but
   * is usable on any element while in contenteditable mode.
   */
  [@bs.as "inputmode"]
  [@bs.optional]
  inputMode: string,

  /**
   * Allows you to specify that a standard HTML element should behave like a registered
   * custom built-in element (see Using custom elements for more details).
   */
  [@bs.as "is"]
  [@bs.optional]
  is: string,

  /**
   * The unique, global identifier of an item.
   */
  [@bs.as "itemid"]
  [@bs.optional]
  itemId: string,

  /**
   * Used to add properties to an item. Every HTML element may have an itemprop attribute
   * specified, where an itemprop consists of a name and value pair.
   */
  [@bs.as "itemprop"]
  [@bs.optional]
  itemProp: string,

  /**
   * Properties that are not descendants of an element with the itemscope attribute can be
   * associated with the item using an itemref. It provides a list of element ids (not itemids)
   * with additional properties elsewhere in the document.
   */
  [@bs.as "itemref"]
  [@bs.optional]
  itemRef: string,

  /**
   * itemscope (usually) works along with itemtype to specify that the HTML contained in a
   * block is about a particular item. itemscope creates the Item and defines the scope of
   * the itemtype associated with it. itemtype is a valid URL of a vocabulary (such as schema.org)
   * that describes the item and its properties context.
   */
  [@bs.as "itemscope"]
  [@bs.optional]
  itemScope: string,

  /**
   * Specifies the URL of the vocabulary that will be used to define itemprops (item properties)
   * in the data structure. itemscope is used to set the scope of where in the data structure
   * the vocabulary set by itemtype will be active.
   */
  [@bs.as "itemtype"]
  [@bs.optional]
  itemType: string,

  /**
   * Helps define the language of an element: the language that non-editable elements are in,
   * or the language that editable elements should be written in by the user. The attribute
   * contains one “language tag” (made of hyphen-separated “language subtags”) in the format
   * defined in Tags for Identifying Languages (BCP47). xml:lang has priority over it.
   */
  [@bs.as "lang"]
  [@bs.optional]
  lang: string,

  /**
   * A space-separated list of the part names of the element. Part names allows CSS to select
   * and style specific elements in a shadow tree via the ::part pseudo-element.
   */
  [@bs.as "string"]
  [@bs.optional]
  part: string,
  
  /**
   * Assigns a slot in a shadow DOM shadow tree to an element: An element with a slot attribute
   * is assigned to the slot created by the <slot> element whose name attribute's value matches
   * that slot attribute's value.
   */
  [@bs.as "slot"]
  [@bs.optional]
  slot: string,

  /**
   * An enumerated attribute defines whether the element may be checked for spelling errors.
   * It may have the following values:
   * - true, which indicates that the element should be, if possible, checked for spelling errors;
   * - false, which indicates that the element should not be checked for spelling errors.
   */
  [@bs.as "spellcheck"]
  [@bs.optional]
  spellCheck: bool,

  /**
   * An integer attribute indicating if the element can take input focus (is focusable),
   * if it should participate to sequential keyboard navigation, and if so, at what position.
   * It can take several values:
   * - a negative value means that the element should be focusable, but should not be reachable
   * via sequential keyboard navigation;
   * - 0 means that the element should be focusable and reachable via sequential keyboard
   * navigation, but its relative order is defined by the platform convention;
   * - a positive value means that the element should be focusable and reachable via sequential
   * keyboard navigation; the order in which the elements are focused is the increasing value
   * of the tabindex. If several elements share the same tabindex, their relative order follows
   * their relative positions in the document.
   */
  [@bs.as "tabindex"]
  [@bs.optional]
  tabIndex: int,

  /**
   * Contains a text representing advisory information related to the element it belongs to.
   * Such information can typically, but not necessarily, be presented to the user as a tooltip.
   */
  [@bs.as "title"]
  [@bs.optional]
  title: string,
};