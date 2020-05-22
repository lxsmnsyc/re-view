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
   * Identifies the currently active element when DOM focus
   * is on a composite widget, textbox, group, or application.
   */
  [@bs.as "aria-activedescendant"]
  [@bs.optional]
  activeDescendant: string,

  /**
   * Indicates whether assistive technologies will present all,
   * or only parts of, the changed region based on the change
   * notifications defined by the aria-relevant attribute.
   */
  [@bs.as "aria-atomic"]
  [@bs.optional]
  atomic: bool,

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
  autoComplete: string,

  /**
   * Indicates an element is being modified and that assistive
   * technologies MAY want to wait until the modifications are
   * complete before exposing them to the user.
   */
  [@bs.as "aria-busy"]
  [@bs.optional]
  busy: bool,

  /**
   * Indicates the current "checked" state of checkboxes, radio
   * buttons, and other widgets.
   * @see aria-pressed @see aria-selected.
   * 
   * boolean | 'false' | 'mixed' | 'true'
   */
  [@bs.as "aria-checked"]
  [@bs.optional]
  checked: string,

  /**
   * Defines the total number of columns in a table, grid, or treegrid.
   * @see aria-colindex.
   */
  [@bs.as "aria-colcount"]
  [@bs.optional]
  colCount: int,

  /**
   * Defines an element's column index or position with respect to
   * the total number of columns within a table, grid, or treegrid.
   * @see aria-colcount @see aria-colspan.
   */
  [@bs.as "aria-colindex"]
  [@bs.optional]
  colIndex: int,


  /**
   * Defines the number of columns spanned by a cell or gridcell
   * within a table, grid, or treegrid.
   * @see aria-colindex @see aria-rowspan.
   */
  [@bs.as "aria-colspan"]
  [@bs.optional]
  colSpan: int,


  /**
   * Identifies the element (or elements) whose contents or presence
   * are controlled by the current element.
   * @see aria-owns.
   */
  [@bs.as "aria-controls"]
  [@bs.optional]
  controls: string,

  /**
   * Indicates the element that represents the current item within
   * a container or set of related elements.
   * 
   * 'false' | 'true' | 'page' | 'step' | 'location' | 'date' | 'time';
   */
  [@bs.as "aria-current"]
  [@bs.optional]
  current: string,

  
  /**
   * Identifies the element (or elements) that describes the object.
   * @see aria-labelledby
   */
  [@bs.as "aria-describedby"]
  [@bs.optional]
  describedBy: string,

  /**
   * Identifies the element that provides a detailed, extended
   * description for the object.
   * @see aria-describedby.
   */
  [@bs.as "aria-details"]
  [@bs.optional]
  details: string,

  /**
   * Indicates that the element is perceivable but disabled, so
   * it is not editable or otherwise operable.
   * @see aria-hidden @see aria-readonly.
  */
  [@bs.as "aria-disabled"]
  [@bs.optional]
  disabled: bool,

  /**
   * Indicates what functions can be performed when a dragged 
   * object is released on the drop target.
   * @deprecated in ARIA 1.1
   * 'none' | 'copy' | 'execute' | 'link' | 'move' | 'popup';
   */
  [@bs.as "aria-dropeffect"]
  [@bs.optional]
  dropEffect: string,

  /**
   * Identifies the element that provides an error message for the object.
   * @see aria-invalid @see aria-describedby.
   */
  [@bs.as "aria-errormessage"]
  [@bs.optional]
  errorMessage: string,

  /**
   * Indicates whether the element, or another grouping element it controls,
   * is currently expanded or collapsed.
   */
  [@bs.as "aria-expanded"]
  [@bs.optional]
  expanded: string,

  /**
   * Identifies the next element (or elements) in an alternate reading order
   * of content which, at the user's discretion, allows assistive technology
   * to override the general default of reading in document source order.
   */
  [@bs.as "aria-flowto"]
  [@bs.optional]
  flowTo: string,

  /**
   * Indicates an element's "grabbed" state in a drag-and-drop operation.
   * @deprecated in ARIA 1.1
   */
  [@bs.as "aria-grabbed"]
  [@bs.optional]
  grabbed: bool,

  /**
   * Indicates the availability and type of interactive popup element,
   * such as menu or dialog, that can be triggered by an element.
   * 
   * boolean | 'false' | 'true' | 'menu' | 'listbox' | 'tree' | 'grid' | 'dialog'
   */
  [@bs.as "aria-haspopup"]
  [@bs.optional]
  hasPopup: string,

  /**
   * Indicates whether the element is exposed to an accessibility API.
   * @see aria-disabled.
   */
  [@bs.as "aria-hidden"]
  [@bs.optional]
  hidden: bool,

  /**
   * Indicates the entered value does not conform to the format
   * expected by the application.
   * @see aria-errormessage.
   * boolean | 'false' | 'true' | 'grammar' | 'spelling'
   */
  [@bs.as "aria-invalid"]
  [@bs.optional]
  invalid: string,
  /**
   * Indicates keyboard shortcuts that an author has implemented
   * to activate or give focus to an element.
   */
  [@bs.as "aria-keyshortcuts"]
  [@bs.optional]
  keyShortcuts: string,
  /**
   * Defines a string value that labels the current element.
   * @see aria-labelledby.
   */
  [@bs.as "aria-label"]
  [@bs.optional]
  label: string,
  /**
   * Identifies the element (or elements) that labels the current element.
   * @see aria-describedby.
   */
  [@bs.as "aria-labelledby"]
  [@bs.optional]
  labelledBy: string,
  /**
   * Defines the hierarchical level of an element within a structure.
   */
  [@bs.as "aria-level"]
  [@bs.optional]
  level: string,
  /**
   * Indicates that an element will be updated, and describes the
   * types of updates the user agents, assistive technologies,
   * and user can expect from the live region.
   * 
   * 'off' | 'assertive' | 'polite'
   */
  [@bs.as "aria-live"]
  [@bs.optional]
  live: string,
  /**
   * Indicates whether an element is modal when displayed.
   */
  [@bs.as "aria-modal"]
  [@bs.optional]
  modal: bool,
  /**
   * Indicates whether a text box accepts multiple lines of
   * input or only a single line.
   */
  [@bs.as "aria-multiline"]
  [@bs.optional]
  multiLine: bool,
  /**
   * Indicates that the user may select more than one item
   * from the current selectable descendants.
   */
  [@bs.as "aria-multiselectable"]
  [@bs.optional]
  multiSelectable: bool,
  /**
   * Indicates whether the element's orientation is horizontal,
   * vertical, or unknown/ambiguous.
   * 
   * 'horizontal' | 'vertical'
   */
  [@bs.as "aria-orientation"]
  [@bs.optional]
  orientation: string,

  /**
   * Identifies an element (or elements) in order to define a visual,
   * functional, or contextual parent/child relationship between DOM
   * elements where the DOM hierarchy cannot be used to represent
   * the relationship.
   * @see aria-controls.
   */
  [@bs.as "aria-owns"]
  [@bs.optional]
  owns: string,

  /**
   * Defines a short hint (a word or short phrase) intended to aid
   * the user with data entry when the control has no value.
   * A hint could be a sample value or a brief description of
   * the expected format.
   */
  [@bs.as "aria-placeholder"]
  [@bs.optional]
  placeholder: string,

  /**
   * Defines an element's number or position in the current set
   * of listitems or treeitems. Not required if all elements in
   * the set are present in the DOM.
   * @see aria-setsize.
   */
  [@bs.as "aria-posinset"]
  [@bs.optional]
  posInset: int,

  /**
   * Indicates the current "pressed" state of toggle buttons.
   * @see aria-checked @see aria-selected.
   */
  [@bs.as "aria-pressed"]
  [@bs.optional]
  pressed: string,

  /**
    * Indicates that the element is not editable, but is otherwise operable.
    * @see aria-disabled.
    */
  [@bs.as "aria-readonly"]
  [@bs.optional]
  readonly: bool,

  /**
   * Indicates what notifications the user agent will trigger
   * when the accessibility tree within a live region is modified.
   * @see aria-atomic.
   * 
   * 'additions' | 'additions text' | 'all' | 'removals' | 'text'
   */
  [@bs.as "aria-relevant"]
  [@bs.optional]
  relevant: string,

  /**
   * Indicates that user input is required on the element
   * before a form may be submitted.
   */
  [@bs.as "aria-required"]
  [@bs.optional]
  required: bool,

  /**
   * Defines a human-readable, author-localized description
   * for the role of an element.
   */
  [@bs.as "aria-roledescription"]
  [@bs.optional]
  roleDescription: string,

  /**
   * Defines the total number of rows in a table, grid, or treegrid.
   * @see aria-rowindex.
   */
  [@bs.as "aria-rowcount"]
  [@bs.optional]
  rowCount: int,

  /**
   * Defines an element's row index or position with respect to the
   * total number of rows within a table, grid, or treegrid.
   * @see aria-rowcount @see aria-rowspan.
   */
  [@bs.as "aria-rowindex"]
  [@bs.optional]
  rowIndex: int,

  /**
   * Defines the number of rows spanned by a cell or gridcell within
   * a table, grid, or treegrid.
   * @see aria-rowindex @see aria-colspan.
   */
  [@bs.as "aria-rowspan"]
  [@bs.optional]
  rowSpan: int,

  /**
   * Indicates the current "selected" state of various widgets.
   * @see aria-checked @see aria-pressed.
   */
  [@bs.as "aria-selected"]
  [@bs.optional]
  selected: bool,

  /**
   * Defines the number of items in the current set of listitems
   * or treeitems. Not required if all elements in the set are
   * present in the DOM.
   * @see aria-posinset.
   */
  [@bs.as "aria-setsize"]
  [@bs.optional]
  setSize: int,

  /**
   * Indicates if items in a table or grid are sorted in ascending
   * or descending order.
   * 
   * 'none' | 'ascending' | 'descending' | 'other'
   */
  [@bs.as "aria-sort"]
  [@bs.optional]
  sort: string,

  /**
   * Defines the maximum allowed value for a range widget. 
   */
  [@bs.as "aria-valuemax"]
  [@bs.optional]
  valueMax: int,

  /**
   * Defines the minimum allowed value for a range widget.
   */
  [@bs.as "aria-valuemin"]
  [@bs.optional]
  valueMin: int,

  /**
   * Defines the current value for a range widget.
   * @see aria-valuetext.
   */
  [@bs.as "aria-valuenow"]
  [@bs.optional]
  valueNow: int,

  /**
   * Defines the human readable text alternative of
   * aria-valuenow for a range widget.
   */
  [@bs.as "aria-valuetext"]
  [@bs.optional]
  valueText: string,
};