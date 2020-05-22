/**
 * Main root
 */
module Html = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    xmlns: string,
  };

  let tag = "html";
});

/**
 * Document metadata
 */
module Base = HTMLElement.WithoutChild({
  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    href: string,

    [@bs.optional]
    target: string,
  };

  let tag = "base";
});

module Head = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "head";
});

module Link = HTMLElement.WithoutChild({
  [@bs.deriving abstract]
  type t = {
    [@bs.as "as"]
    [@bs.optional]
    as_: string,

    [@bs.as "crossorigin"]
    [@bs.optional]
    crossOrigin: string,

    [@bs.optional]
    disabled: bool,

    [@bs.optional]
    href: string,

    [@bs.as "hreflang"]
    [@bs.optional]
    hrefLang: string,

    [@bs.optional]
    media: string,

    [@bs.optional]
    rel: string,

    [@bs.optional]
    sizes: string,

    [@bs.optional]
    title: string,

    [@bs.as "type"]
    [@bs.optional]
    type_: string,
  };

  let tag = "link";
});

module Meta = HTMLElement.WithoutChild({
  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    charset: string,

    [@bs.optional]
    content: string,

    [@bs.as "http-equiv"]
    [@bs.optional]
    httpEquiv: string,

    [@bs.optional]
    name: string,
  };

  let tag = "meta";
});

module Style = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t = {
    [@bs.as "type"]
    [@bs.optional]
    type_: string,

    [@bs.optional]
    media: string,

    [@bs.optional]
    nonce: string,

    [@bs.optional]
    title: string,
  };

  let tag = "style";
});

module Title = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "title";
});

/**
 * Sectioning Root
 */
module Body = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "body";
});

/**
 * Content Sectioning
 */
module Address = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "address";
});

module Article = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "article";
});

module Footer = HTMLElement.WithoutChild({
  [@bs.deriving abstract]
  type t;

  let tag = "footer";
});

module Header = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "header";
});

module Heading1 = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "h1";
});

module Heading2 = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "h2";
});

module Heading3 = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "h3";
});

module Heading4 = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "h4";
});

module Heading5 = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "h5";
});

module Heading6 = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "h6";
});

module HeadingGroup = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "hgroup";
});

module Main = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "main";
});

module Navigation = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "nav";
});

module Section = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "section";
});

/**
 * Text content
 */
module BlockQuote = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    cite: string,
  };

  let tag = "blockquote";
});

module DescriptionDefinition = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "dd";
});

module Division = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "div";
});

module DescriptionList = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "dl";
});

module DescriptionTerm = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "dt";
});

module FigureCaption = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "figcaption";
});

module Figure = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "figure";
});

module HorizontalRule = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "hr";
});

module ListItem = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    value: string,
  };

  let tag = "li";
});

module OrderedList = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    reversed: bool,

    [@bs.optional]
    start: int,

    [@bs.as "type"]
    [@bs.optional]
    type_: string,
  };

  let tag = "ol";
});

module Paragraph = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "p";
});

module Preformat = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "pre";
});

module UnorderedList = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "ul";
});

/**
 * Inline text semantics
 */
module Anchor = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    download: string,

    [@bs.optional]
    href: string,

    [@bs.as "hreflang"]
    [@bs.optional]
    hrefLang: string,

    [@bs.optional]
    ping: string,

    [@bs.optional]
    rel: string,

    [@bs.optional]
    target: string,

    [@bs.as "type"]
    [@bs.optional]
    type_: string,
  };

  let tag = "a";
});

module Abbreviation = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "abbr";
});

module BidirectionalIsolate = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "bdi";
});

module BidirectionalOverride = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    dir: string,
  };

  let tag = "bdo";
});

module Break = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "br";
});

module Citation = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "cite";
});

module Code = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "code";
});

module Data = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    value: string,
  };

  let tag = "data";
});

module Definition = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "dfn";
});

module Emphasis = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "em";
});

module Italic = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "i";
});

module KeyboardInput = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "kbd";
});

module Mark = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "mark";
});

module Quote = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    cite: string,
  };

  let tag = "q";
});

module Ruby = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "ruby";
});

module RubyText = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "rt";
});

module RubyTextContainer = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "rtc";
});

module Sample = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "samp";
});

module Ruby = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "ruby";
});

module Small = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "small";
});

module Span = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "span";
});

module StrikeThrough = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "s";
});

module Strong = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "strong";
});


module Subscript = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "sub";
});

module Superscript = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "sup";
});

module Time = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t = {
    [@bs.as "datetime"]
    [@bs.optional]
    dateTime: string,
  };

  let tag = "time";
});

module Variable = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "var";
});

module WordBreak = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t;

  let tag = "wbr";
});

/**
 * Image and multimedia
 */
module Area = HTMLElement.WithoutChild({
  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    alt: string,

    [@bs.optional]
    coords: string,

    [@bs.optional]
    download: string,

    [@bs.optional]
    href: string,

    [@bs.as "hreflang"]
    [@bs.optional]
    hrefLang: string,

    [@bs.optional]
    ping: string,

    [@bs.optional]
    rel: string,

    [@bs.optional]
    shape: string,

    [@bs.optional]
    target: string,
  };

  let tag = "area";
});

module Audio = HTMLElement.WithoutChild({
  [@bs.deriving abstract]
  type t = {
    [@bs.as "autoplay"]
    [@bs.optional]
    autoPlay: boolean,

    [@bs.optional]
    controls: boolean,

    [@bs.as "crossorigin"]
    [@bs.optional]
    crossOrigin: string,

    [@bs.optional]
    loop: bool,

    [@bs.optional]
    muted: bool,

    [@bs.optional]
    preload: string,

    [@bs.optional]
    src: string,
  };

  let tag = "audio";
});

module Image = HTMLElement.WithoutChild({
  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    alt: string,

    [@bs.as "crossorigin"]
    [@bs.optional]
    crossOrigin: string,

    [@bs.optional]
    decoding: string,

    [@bs.optional]
    height: int,

    [@bs.as "ismap"]
    [@bs.optional]
    isMap: boolean,

    [@bs.optional]
    loading: string,

    [@bs.optional]
    sizes: string,

    [@bs.optional]
    src: string,

    [@bs.as "srcset"]
    [@bs.optional]
    srcSet: string,

    [@bs.optional]
    width: int,

    [@bs.as "usemap"]
    [@bs.optional]
    useMap: string,
  };

  let tag = "img";
});

module Map = HTMLElement.WithChild({
  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    name: string,
  };

  let tag = "map"
});

module Track = HTMLElement.WithoutChild({
  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    default: bool,

    [@bs.optional]
    kind: string,

    [@bs.optional]
    label: string,

    [@bs.optional]
    src: string,

    [@bs.as "srclang"]
    [@bs.optional]
    srcLang: string,
  };

  let tag = "track"
});
