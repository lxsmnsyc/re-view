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
module Reconciler: Types.Reconciler = {
  type t;

  [@bs.module "./bindings/dom.js"]
  external createInstance: (string, 'props, string, 'fiber) => t = "createInstance";
  [@bs.module "./bindings/dom.js"]
  external appendChild: (t, t, int, 'fiber) => unit = "appendChild";
  [@bs.module "./bindings/dom.js"]
  external removeChild: (t, t, int, 'fiber) => unit = "removeChild";
  [@bs.module "./bindings/dom.js"]
  external commitUpdate: (t, 'props, 'props, int, 'fiber) => unit = "commitUpdate";
};

module DomCore = Engine.Make(Reconciler);

let render = DomCore.Core.render;

/**
 * Hooks
 */
let useCallback = DomCore.useCallback;
let useConstant = DomCore.useConstant;
let useContext = DomCore.useContext;
let useEffect = DomCore.useEffect;
let useForceUpdate = DomCore.useForceUpdate;
let useIdentifier = DomCore.useIdentifier;
let useLayoutEffect = DomCore.useLayoutEffect;
let useMemo = DomCore.useMemo;
let useMutable = DomCore.useMutable;
let useReducer = DomCore.useReducer;
let useState = DomCore.useState;

/**
 * Components
 */
module Context = DomCore.Context;
module Fragment = DomCore.Fragment;
module ErrorBoundary = DomCore.ErrorBoundary;
module Basic = DomCore.Basic;
module MemoBasic = DomCore.MemoBasic;
module Component = DomCore.Component;
module Memo = DomCore.Memo;


module Element = {
  [@bs.deriving abstract]
  type attributes = {
    /**
     * Aria
     */
    /**
     * Identifies the currently active element when DOM focus
     * is on a composite widget, textbox, group, or application.
     */
    [@bs.as "aria-activedescendant"]
    [@bs.optional]
    ariaActiveDescendant: string,

    /**
     * Indicates whether assistive technologies will present all,
     * or only parts of, the changed region based on the change
     * notifications defined by the aria-relevant attribute.
     */
    [@bs.as "aria-atomic"]
    [@bs.optional]
    ariaAtomic: bool,

    /**
     * Indicates whether inputting text could trigger display of
     * one or more predictions of the user's intended value for an
     * input and specifies how predictions would be
     * presented if they are made.
     * 
     * 'none' | 'inline' | 'list' | 'both'
     */
    [@bs.as "aria-autocomplete"]
    [@bs.optional]
    ariaAutoComplete: string,

    /**
     * Indicates an element is being modified and that assistive
     * technologies MAY want to wait until the modifications are
     * complete before exposing them to the user.
     */
    [@bs.as "aria-busy"]
    [@bs.optional]
    ariaBusy: bool,

    /**
     * Indicates the current "checked" state of checkboxes, radio
     * buttons, and other widgets.
     * @see aria-pressed @see aria-selected.
     * 
     * boolean | 'false' | 'mixed' | 'true'
     */
    [@bs.as "aria-checked"]
    [@bs.optional]
    ariaChecked: string,

    /**
     * Defines the total number of columns in a table, grid, or treegrid.
     * @see aria-colindex.
     */
    [@bs.as "aria-colcount"]
    [@bs.optional]
    ariaColCount: int,

    /**
     * Defines an element's column index or position with respect to
     * the total number of columns within a table, grid, or treegrid.
     * @see aria-colcount @see aria-colspan.
     */
    [@bs.as "aria-colindex"]
    [@bs.optional]
    ariaColIndex: int,


    /**
     * Defines the number of columns spanned by a cell or gridcell
     * within a table, grid, or treegrid.
     * @see aria-colindex @see aria-rowspan.
     */
    [@bs.as "aria-colspan"]
    [@bs.optional]
    ariaColSpan: int,

    /**
     * Identifies the element (or elements) whose contents or presence
     * are controlled by the current element.
     * @see aria-owns.
     */
    [@bs.as "aria-controls"]
    [@bs.optional]
    ariaControls: string,

    /**
     * Indicates the element that represents the current item within
     * a container or set of related elements.
     * 
     * 'false' | 'true' | 'page' | 'step' | 'location' | 'date' | 'time';
     */
    [@bs.as "aria-current"]
    [@bs.optional]
    ariaCurrent: string,
    
    /**
     * Identifies the element (or elements) that describes the object.
     * @see aria-labelledby
     */
    [@bs.as "aria-describedby"]
    [@bs.optional]
    ariaDescribedBy: string,

    /**
     * Identifies the element that provides a detailed, extended
     * description for the object.
     * @see aria-describedby.
     */
    [@bs.as "aria-details"]
    [@bs.optional]
    ariaDetails: string,

    /**
     * Indicates that the element is perceivable but disabled, so
     * it is not editable or otherwise operable.
     * @see aria-hidden @see aria-readonly.
    */
    [@bs.as "aria-disabled"]
    [@bs.optional]
    ariaDisabled: bool,

    /**
     * Indicates what functions can be performed when a dragged 
     * object is released on the drop target.
     * @deprecated in ARIA 1.1
     * 'none' | 'copy' | 'execute' | 'link' | 'move' | 'popup';
     */
    [@bs.as "aria-dropeffect"]
    [@bs.optional]
    ariaDropEffect: string,

    /**
     * Identifies the element that provides an error message for the object.
     * @see aria-invalid @see aria-describedby.
     */
    [@bs.as "aria-errormessage"]
    [@bs.optional]
    ariaErrorMessage: string,

    /**
     * Indicates whether the element, or another grouping element it controls,
     * is currently expanded or collapsed.
     */
    [@bs.as "aria-expanded"]
    [@bs.optional]
    ariaExpanded: string,

    /**
     * Identifies the next element (or elements) in an alternate reading order
     * of content which, at the user's discretion, allows assistive technology
     * to override the general default of reading in document source order.
     */
    [@bs.as "aria-flowto"]
    [@bs.optional]
    ariaFlowTo: string,

    /**
     * Indicates an element's "grabbed" state in a drag-and-drop operation.
     * @deprecated in ARIA 1.1
     */
    [@bs.as "aria-grabbed"]
    [@bs.optional]
    ariaGrabbed: bool,

    /**
     * Indicates the availability and type of interactive popup element,
     * such as menu or dialog, that can be triggered by an element.
     * 
     * boolean | 'false' | 'true' | 'menu' | 'listbox' | 'tree' | 'grid' | 'dialog'
     */
    [@bs.as "aria-haspopup"]
    [@bs.optional]
    ariaHasPopup: string,

    /**
     * Indicates whether the element is exposed to an accessibility API.
     * @see aria-disabled.
     */
    [@bs.as "aria-hidden"]
    [@bs.optional]
    ariaHidden: bool,

    /**
     * Indicates the entered value does not conform to the format
     * expected by the application.
     * @see aria-errormessage.
     * boolean | 'false' | 'true' | 'grammar' | 'spelling'
     */
    [@bs.as "aria-invalid"]
    [@bs.optional]
    ariaInvalid: string,
    /**
     * Indicates keyboard shortcuts that an author has implemented
     * to activate or give focus to an element.
     */
    [@bs.as "aria-keyshortcuts"]
    [@bs.optional]
    ariaKeyShortcuts: string,
    /**
     * Defines a string value that labels the current element.
     * @see aria-labelledby.
     */
    [@bs.as "aria-label"]
    [@bs.optional]
    ariaLabel: string,
    /**
     * Identifies the element (or elements) that labels the current element.
     * @see aria-describedby.
     */
    [@bs.as "aria-labelledby"]
    [@bs.optional]
    ariaLabelledBy: string,
    /**
     * Defines the hierarchical level of an element within a structure.
     */
    [@bs.as "aria-level"]
    [@bs.optional]
    ariaLevel: string,
    /**
     * Indicates that an element will be updated, and describes the
     * types of updates the user agents, assistive technologies,
     * and user can expect from the live region.
     * 
     * 'off' | 'assertive' | 'polite'
     */
    [@bs.as "aria-live"]
    [@bs.optional]
    ariaLive: string,
    /**
     * Indicates whether an element is modal when displayed.
     */
    [@bs.as "aria-modal"]
    [@bs.optional]
    ariaModal: bool,
    /**
     * Indicates whether a text box accepts multiple lines of
     * input or only a single line.
     */
    [@bs.as "aria-multiline"]
    [@bs.optional]
    ariaMultiLine: bool,
    /**
     * Indicates that the user may select more than one item
     * from the current selectable descendants.
     */
    [@bs.as "aria-multiselectable"]
    [@bs.optional]
    ariaMultiSelectable: bool,
    /**
     * Indicates whether the element's orientation is horizontal,
     * vertical, or unknown/ambiguous.
     * 
     * 'horizontal' | 'vertical'
     */
    [@bs.as "aria-orientation"]
    [@bs.optional]
    ariaOrientation: string,

    /**
     * Identifies an element (or elements) in order to define a visual,
     * functional, or contextual parent/child relationship between DOM
     * elements where the DOM hierarchy cannot be used to represent
     * the relationship.
     * @see aria-controls.
     */
    [@bs.as "aria-owns"]
    [@bs.optional]
    ariaOwns: string,

    /**
     * Defines a short hint (a word or short phrase) intended to aid
     * the user with data entry when the control has no value.
     * A hint could be a sample value or a brief description of
     * the expected format.
     */
    [@bs.as "aria-placeholder"]
    [@bs.optional]
    ariaPlaceholder: string,

    /**
     * Defines an element's number or position in the current set
     * of listitems or treeitems. Not required if all elements in
     * the set are present in the DOM.
     * @see aria-setsize.
     */
    [@bs.as "aria-posinset"]
    [@bs.optional]
    ariaPosInset: int,

    /**
     * Indicates the current "pressed" state of toggle buttons.
     * @see aria-checked @see aria-selected.
     */
    [@bs.as "aria-pressed"]
    [@bs.optional]
    ariaPressed: string,

    /**
      * Indicates that the element is not editable, but is otherwise operable.
      * @see aria-disabled.
      */
    [@bs.as "aria-readonly"]
    [@bs.optional]
    ariaReadonly: bool,

    /**
     * Indicates what notifications the user agent will trigger
     * when the accessibility tree within a live region is modified.
     * @see aria-atomic.
     * 
     * 'additions' | 'additions text' | 'all' | 'removals' | 'text'
     */
    [@bs.as "aria-relevant"]
    [@bs.optional]
    ariaRelevant: string,

    /**
     * Indicates that user input is required on the element
     * before a form may be submitted.
     */
    [@bs.as "aria-required"]
    [@bs.optional]
    ariaRequired: bool,

    /**
     * Defines a human-readable, author-localized description
     * for the role of an element.
     */
    [@bs.as "aria-roledescription"]
    [@bs.optional]
    ariaRoleDescription: string,

    /**
     * Defines the total number of rows in a table, grid, or treegrid.
     * @see aria-rowindex.
     */
    [@bs.as "aria-rowcount"]
    [@bs.optional]
    ariaRowCount: int,

    /**
     * Defines an element's row index or position with respect to the
     * total number of rows within a table, grid, or treegrid.
     * @see aria-rowcount @see aria-rowspan.
     */
    [@bs.as "aria-rowindex"]
    [@bs.optional]
    ariaRowIndex: int,

    /**
     * Defines the number of rows spanned by a cell or gridcell within
     * a table, grid, or treegrid.
     * @see aria-rowindex @see aria-colspan.
     */
    [@bs.as "aria-rowspan"]
    [@bs.optional]
    ariaRowSpan: int,

    /**
     * Indicates the current "selected" state of various widgets.
     * @see aria-checked @see aria-pressed.
     */
    [@bs.as "aria-selected"]
    [@bs.optional]
    ariaSelected: bool,

    /**
     * Defines the number of items in the current set of listitems
     * or treeitems. Not required if all elements in the set are
     * present in the DOM.
     * @see aria-posinset.
     */
    [@bs.as "aria-setsize"]
    [@bs.optional]
    ariaSetSize: int,

    /**
     * Indicates if items in a table or grid are sorted in ascending
     * or descending order.
     * 
     * 'none' | 'ascending' | 'descending' | 'other'
     */
    [@bs.as "aria-sort"]
    [@bs.optional]
    ariaSort: string,

    /**
     * Defines the maximum allowed value for a range widget. 
     */
    [@bs.as "aria-valuemax"]
    [@bs.optional]
    ariaValueMax: int,

    /**
     * Defines the minimum allowed value for a range widget.
     */
    [@bs.as "aria-valuemin"]
    [@bs.optional]
    ariaValueMin: int,

    /**
     * Defines the current value for a range widget.
     * @see aria-valuetext.
     */
    [@bs.as "aria-valuenow"]
    [@bs.optional]
    ariaValueNow: int,

    /**
     * Defines the human readable text alternative of
     * aria-valuenow for a range widget.
     */
    [@bs.as "aria-valuetext"]
    [@bs.optional]
    ariaValueText: string,

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


    [@bs.optional]
    xmlns: string,

    [@bs.optional]
    href: string,

    [@bs.optional]
    target: string,

    [@bs.as "as"]
    [@bs.optional]
    as_: string,

    [@bs.as "crossorigin"]
    [@bs.optional]
    crossOrigin: string,

    [@bs.optional]
    disabled: bool,

    [@bs.as "hreflang"]
    [@bs.optional]
    hrefLang: string,

    [@bs.optional]
    media: string,

    [@bs.optional]
    rel: string,

    [@bs.optional]
    sizes: string,

    [@bs.as "type"]
    [@bs.optional]
    type_: string,

    [@bs.optional]
    charset: string,

    [@bs.optional]
    content: string,

    [@bs.as "http-equiv"]
    [@bs.optional]
    httpEquiv: string,

    [@bs.optional]
    name: string,

    [@bs.optional]
    nonce: string,

    [@bs.optional]
    cite: string,

    [@bs.optional]
    value: string,

    [@bs.optional]
    reversed: bool,

    [@bs.optional]
    start: int,

    [@bs.optional]
    download: string,

    [@bs.optional]
    ping: string,

    [@bs.as "datetime"]
    [@bs.optional]
    dateTime: string,

    [@bs.optional]
    alt: string,

    [@bs.optional]
    coords: string,

    [@bs.optional]
    shape: string,

    [@bs.as "autoplay"]
    [@bs.optional]
    autoPlay: bool,

    [@bs.optional]
    controls: bool,

    [@bs.optional]
    loop: bool,

    [@bs.optional]
    muted: bool,

    [@bs.optional]
    preload: string,

    [@bs.optional]
    src: string,

    [@bs.optional]
    decoding: string,

    [@bs.optional]
    height: int,

    [@bs.as "ismap"]
    [@bs.optional]
    isMap: bool,

    [@bs.optional]
    loading: string,

    [@bs.as "srcset"]
    [@bs.optional]
    srcSet: string,

    [@bs.optional]
    width: int,

    [@bs.as "usemap"]
    [@bs.optional]
    useMap: string,

    [@bs.optional]
    default: bool,

    [@bs.optional]
    kind: string,

    [@bs.optional]
    label: string,

    [@bs.as "srclang"]
    [@bs.optional]
    srcLang: string,

    [@bs.as "playsinline"]
    [@bs.optional]
    playsInline: bool,

    [@bs.optional]
    poster: string,

    [@bs.optional]
    allow: string,

    [@bs.as "referrerpolicy"]
    [@bs.optional]
    referrerPolicy: string,

    [@bs.optional]
    sandbox: string,

    [@bs.as "srcdoc"]
    [@bs.optional]
    srcDoc: string,

    [@bs.optional]
    data: string,

    [@bs.optional]
    form: string,

    [@bs.as "typemustmatch"]
    [@bs.optional]
    typeMustMatch: bool,

    [@bs.optional]
    async: bool,

    [@bs.optional]
    defer: bool,

    [@bs.optional]
    integrity: string,

    [@bs.as "nomodule"]
    [@bs.optional]
    noModule: bool,
  };

  type props = {
    tag: string,
    attributes: attributes,
    children: Types.Children.t,
  };

  let make: Types.Component.t(props) = ({ ref }, props): option(Types.Element.t) => {
    DomCore.Host.make({ ref, key: None }, {
      constructor: props.tag,
      attributes: Opaque.transform(props.attributes),
      children: props.children,
    });
  };
};

module Text = {
  let make = (value: string) => {
    DomCore.Host.make({ key: None, ref: None }, {
      constructor: "text",
      attributes: Opaque.transform(
        Element.attributes(~value=value, ()),
      ),
      children: [||],
    });
  };
};

[@bs.val] external requestAnimationFrame: (float => unit) => unit = "window.requestAnimationFrame";
[@bs.val] external now: unit => float = "performance.now";

let rec loop = (time) => {
  let current = now();
  DomCore.workLoop(() => current -. time);
  requestAnimationFrame(loop);
};

requestAnimationFrame(loop);