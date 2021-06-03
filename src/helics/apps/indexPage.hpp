/*
Copyright (c) 2017-2021,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable
Energy, LLC.  See the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

namespace helics {
namespace webserver {

    static const char* style =
        R"raw(<style>
html {
  font-size: 100%;
  overflow-y: scroll;
  -webkit-text-size-adjust: 100%;
  -ms-text-size-adjust: 100%;
}

body {
  color: #444;
  font-family: Georgia, Palatino, 'Palatino Linotype', Times, 'Times New Roman', serif;
  font-size: 12px;
  line-height: 1.7;
  padding: 1em;
  margin: auto;
  max-width: 42em;
  background: #fefefe;
}

a {
  color: #0645ad;
  text-decoration: none;
}

a:visited {
  color: #0b0080;
}

a:hover {
  color: #06e;
}

a:active {
  color: #faa700;
}

a:focus {
  outline: thin dotted;
}

*::-moz-selection {
  background: rgba(255, 255, 0, 0.3);
  color: #000;
}

*::selection {
  background: rgba(255, 255, 0, 0.3);
  color: #000;
}

a::-moz-selection {
  background: rgba(255, 255, 0, 0.3);
  color: #0645ad;
}

a::selection {
  background: rgba(255, 255, 0, 0.3);
  color: #0645ad;
}

p {
  margin: 1em 0;
}

img {
  max-width: 100%;
}

h1, h2, h3, h4, h5, h6 {
  color: #111;
  line-height: 125%;
  margin-top: 2em;
  font-weight: normal;
}

h4, h5, h6 {
  font-weight: bold;
}

h1 {
  font-size: 2.5em;
}

h2 {
  font-size: 2em;
}

h3 {
  font-size: 1.5em;
}

h4 {
  font-size: 1.2em;
}

h5 {
  font-size: 1em;
}

h6 {
  font-size: 0.9em;
}

blockquote {
  color: #666666;
  margin: 0;
  padding-left: 3em;
  border-left: 0.5em #EEE solid;
}

hr {
  display: block;
  height: 2px;
  border: 0;
  border-top: 1px solid #aaa;
  border-bottom: 1px solid #eee;
  margin: 1em 0;
  padding: 0;
}

pre, code, kbd, samp {
  color: #000;
  font-family: monospace, monospace;
  _font-family: 'courier new', monospace;
  font-size: 0.98em;
}

pre {
  white-space: pre;
  white-space: pre-wrap;
  word-wrap: break-word;
}

b, strong {
  font-weight: bold;
}

dfn {
  font-style: italic;
}

ins {
  background: #ff9;
  color: #000;
  text-decoration: none;
}

mark {
  background: #ff0;
  color: #000;
  font-style: italic;
  font-weight: bold;
}

sub, sup {
  font-size: 75%;
  line-height: 0;
  position: relative;
  vertical-align: baseline;
}

sup {
  top: -0.5em;
}

sub {
  bottom: -0.25em;
}

ul, ol {
  margin: 1em 0;
  padding: 0 0 0 2em;
}

li p:last-child {
  margin-bottom: 0;
}

ul ul, ol ol {
  margin: .3em 0;
}

dl {
  margin-bottom: 1em;
}

dt {
  font-weight: bold;
  margin-bottom: .8em;
}

dd {
  margin: 0 0 .8em 2em;
}

dd:last-child {
  margin-bottom: 0;
}

img {
  border: 0;
  -ms-interpolation-mode: bicubic;
  vertical-align: middle;
}

figure {
  display: block;
  text-align: center;
  margin: 1em 0;
}

figure img {
  border: none;
  margin: 0 auto;
}

figcaption {
  font-size: 0.8em;
  font-style: italic;
  margin: 0 0 .8em;
}

table {
  margin-bottom: 2em;
  border-bottom: 1px solid #ddd;
  border-right: 1px solid #ddd;
  border-spacing: 0;
  border-collapse: collapse;
}

table th {
  padding: .2em 1em;
  background-color: #eee;
  border-top: 1px solid #ddd;
  border-left: 1px solid #ddd;
}

table td {
  padding: .2em 1em;
  border-top: 1px solid #ddd;
  border-left: 1px solid #ddd;
  vertical-align: top;
}

.author {
  font-size: 1.2em;
  text-align: center;
}

@media only screen and (min-width: 480px) {
  body {
    font-size: 14px;
  }
}
@media only screen and (min-width: 768px) {
  body {
    font-size: 16px;
  }
}
@media print {
  * {
    background: transparent !important;
    color: black !important;
    filter: none !important;
    -ms-filter: none !important;
  }

  body {
    font-size: 12pt;
    max-width: 100%;
  }

  a, a:visited {
    text-decoration: underline;
  }

  hr {
    height: 1px;
    border: 0;
    border-bottom: 1px solid black;
  }

  a[href]:after {
    content: " (" attr(href) ")";
  }

  abbr[title]:after {
    content: " (" attr(title) ")";
  }

  .ir a:after, a[href^="javascript:"]:after, a[href^="#"]:after {
    content: "";
  }

  pre, blockquote {
    border: 1px solid #999;
    padding-right: 1em;
    page-break-inside: avoid;
  }

  tr, img {
    page-break-inside: avoid;
  }

  img {
    max-width: 100% !important;
  }

  @page :left {
    margin: 15mm 20mm 15mm 10mm;
}

  @page :right {
    margin: 15mm 10mm 15mm 20mm;
}

  p, h2, h3 {
    orphans: 3;
    widows: 3;
  }

  h2, h3 {
    page-break-after: avoid;
  }
}
    </style>
    )raw";

    static const char* indexPage1 =
        R"raw(<html>
  <head>
    <meta charset="utf-8" />
    <title>HELICS Web server interface</title>
    )raw";

    static const char* indexPage2 =
        R"raw(</head>

  <body>
    )raw";

    static const char* svg1 = R"raw(<svg version = "1.1" id = "Layer_1" xmlns =
                                    "http://www.w3.org/2000/svg" xmlns:
    xlink = "http://www.w3.org/1999/xlink" x = "0px" y = "0px"
     viewBox="0 0 432 140.1" style="enable-background:new 0 0 432 140.1;" xml:space="preserve" width="20%">
<style type="text/css">
    .st0{fill:#231F20;}
    .st1{fill:url(#SVGID_1_);}
    .st2{fill:url(#SVGID_2_);}
    .st3{fill:url(#SVGID_3_);}
    .st4{fill:url(#SVGID_4_);}
    .st5{fill:url(#SVGID_5_);}
    .st6{fill:#1D1D1D;}
</style>
<g>
    <g>
        <path class="st0" d="M129.1,118.5c0.2,0.3,0.4,0.6,0.7,0.8c0.3,0.2,0.6,0.3,1,0.4c0.4,0.1,0.8,0.1,1.1,0.1c0.3,0,0.6,0,0.9-0.1
            c0.3,0,0.6-0.1,0.9-0.3c0.3-0.1,0.5-0.3,0.7-0.5c0.2-0.2,0.3-0.5,0.3-0.8c0-0.4-0.1-0.7-0.4-0.9c-0.2-0.2-0.5-0.4-0.9-0.6
            c-0.4-0.2-0.8-0.3-1.3-0.4s-1-0.2-1.5-0.4c-0.5-0.1-1-0.3-1.5-0.5c-0.5-0.2-0.9-0.4-1.3-0.7c-0.4-0.3-0.7-0.7-0.9-1.1
            c-0.2-0.4-0.4-1-0.4-1.6c0-0.7,0.1-1.3,0.4-1.8c0.3-0.5,0.7-0.9,1.2-1.3c0.5-0.3,1-0.6,1.6-0.8c0.6-0.2,1.2-0.2,1.8-0.2
            c0.7,0,1.4,0.1,2.1,0.2c0.7,0.2,1.2,0.4,1.7,0.8c0.5,0.4,0.9,0.8,1.2,1.4c0.3,0.6,0.4,1.2,0.4,2H134c0-0.4-0.1-0.7-0.3-1
            c-0.1-0.3-0.3-0.5-0.6-0.6c-0.2-0.2-0.5-0.3-0.8-0.3c-0.3-0.1-0.7-0.1-1-0.1c-0.2,0-0.5,0-0.7,0.1c-0.2,0.1-0.5,0.1-0.7,0.3
            c-0.2,0.1-0.4,0.3-0.5,0.5c-0.1,0.2-0.2,0.4-0.2,0.7c0,0.3,0.1,0.5,0.2,0.6c0.1,0.2,0.3,0.3,0.6,0.5c0.3,0.1,0.7,0.3,1.2,0.4
            c0.5,0.1,1.2,0.3,2.1,0.5c0.3,0.1,0.6,0.1,1.1,0.3c0.5,0.1,0.9,0.3,1.3,0.6c0.4,0.3,0.8,0.7,1.2,1.2c0.3,0.5,0.5,1.1,0.5,1.9
            c0,0.6-0.1,1.2-0.4,1.7c-0.2,0.5-0.6,1-1.1,1.4c-0.5,0.4-1.1,0.7-1.8,0.9s-1.5,0.3-2.5,0.3c-0.8,0-1.5-0.1-2.2-0.3
            c-0.7-0.2-1.3-0.5-1.9-0.9c-0.5-0.4-1-0.9-1.3-1.5s-0.5-1.3-0.5-2.2h2.9C128.8,117.8,128.9,118.2,129.1,118.5z"/>
        <path class="st0" d="M139,110.5v-2.2h2.7v2.2H139z M141.7,112v9.9H139V112H141.7z"/>
        <path class="st0" d="M146.4,112v1.3h0c0.4-0.5,0.8-0.9,1.3-1.2c0.5-0.3,1.1-0.4,1.7-0.4c0.6,0,1.2,0.1,1.7,0.4s0.9,0.7,1.2,1.3
            c0.3-0.4,0.7-0.8,1.2-1.1c0.5-0.3,1.1-0.5,1.8-0.5c0.5,0,1,0.1,1.5,0.2c0.4,0.1,0.8,0.3,1.1,0.6c0.3,0.3,0.6,0.6,0.7,1.1
            c0.2,0.5,0.3,1,0.3,1.6v6.6h-2.7v-5.6c0-0.3,0-0.6,0-0.9c0-0.3-0.1-0.5-0.2-0.8c-0.1-0.2-0.3-0.4-0.5-0.5
            c-0.2-0.1-0.5-0.2-0.9-0.2s-0.7,0.1-0.9,0.2c-0.2,0.1-0.4,0.3-0.6,0.6c-0.1,0.2-0.2,0.5-0.3,0.8c0,0.3-0.1,0.6-0.1,0.9v5.5h-2.7
            v-5.5c0-0.3,0-0.6,0-0.9c0-0.3-0.1-0.5-0.2-0.8c-0.1-0.2-0.3-0.4-0.5-0.6c-0.2-0.1-0.6-0.2-1-0.2c-0.1,0-0.3,0-0.5,0.1
            c-0.2,0.1-0.4,0.2-0.6,0.3c-0.2,0.2-0.4,0.4-0.5,0.7c-0.1,0.3-0.2,0.7-0.2,1.2v5.7h-2.7V112H146.4z"/>
        <path class="st0" d="M167.8,121.9v-1.4h-0.1c-0.3,0.6-0.8,1-1.3,1.2c-0.5,0.3-1.1,0.4-1.7,0.4c-0.7,0-1.3-0.1-1.8-0.3
            c-0.5-0.2-0.8-0.5-1.1-0.8c-0.3-0.3-0.5-0.8-0.6-1.3c-0.1-0.5-0.2-1.1-0.2-1.7V112h2.7v5.6c0,0.8,0.1,1.4,0.4,1.8
            c0.3,0.4,0.7,0.6,1.4,0.6c0.7,0,1.3-0.2,1.6-0.7c0.3-0.4,0.5-1.2,0.5-2.2V112h2.7v9.9H167.8z"/>
        <path class="st0" d="M175.2,108.3v13.6h-2.7v-13.6H175.2z"/>
        <path class="st0" d="M177.2,115.1c0-0.6,0.2-1.2,0.5-1.6c0.3-0.4,0.6-0.8,1.1-1c0.4-0.3,0.9-0.4,1.5-0.5c0.5-0.1,1.1-0.2,1.6-0.2
            c0.5,0,1,0,1.5,0.1c0.5,0.1,1,0.2,1.4,0.4c0.4,0.2,0.8,0.5,1,0.8c0.3,0.4,0.4,0.8,0.4,1.4v5.1c0,0.4,0,0.9,0.1,1.3
            c0.1,0.4,0.1,0.7,0.3,0.9h-2.7c-0.1-0.2-0.1-0.3-0.1-0.5c0-0.2-0.1-0.3-0.1-0.5c-0.4,0.4-0.9,0.8-1.5,0.9
            c-0.6,0.2-1.2,0.3-1.8,0.3c-0.5,0-0.9-0.1-1.3-0.2c-0.4-0.1-0.8-0.3-1.1-0.5c-0.3-0.2-0.5-0.5-0.7-0.9c-0.2-0.4-0.3-0.8-0.3-1.3
            c0-0.6,0.1-1,0.3-1.4c0.2-0.4,0.5-0.7,0.8-0.9c0.3-0.2,0.7-0.4,1.1-0.5c0.4-0.1,0.8-0.2,1.2-0.3c0.4-0.1,0.8-0.1,1.2-0.2
            c0.4,0,0.7-0.1,1-0.2c0.3-0.1,0.5-0.2,0.7-0.3c0.2-0.1,0.3-0.4,0.2-0.6c0-0.3,0-0.5-0.1-0.7c-0.1-0.2-0.2-0.3-0.4-0.4
            c-0.2-0.1-0.3-0.2-0.6-0.2c-0.2,0-0.4,0-0.7,0c-0.5,0-1,0.1-1.3,0.3c-0.3,0.2-0.5,0.6-0.5,1.1H177.2z M183.5,117.1
            c-0.1,0.1-0.3,0.2-0.4,0.2c-0.2,0.1-0.4,0.1-0.6,0.1c-0.2,0-0.4,0.1-0.6,0.1c-0.2,0-0.4,0.1-0.6,0.1c-0.2,0-0.4,0.1-0.6,0.2
            c-0.2,0.1-0.4,0.2-0.5,0.3c-0.1,0.1-0.3,0.2-0.4,0.4c-0.1,0.2-0.1,0.4-0.1,0.6c0,0.2,0,0.4,0.1,0.6c0.1,0.2,0.2,0.3,0.4,0.4
            c0.2,0.1,0.3,0.2,0.5,0.2c0.2,0,0.4,0.1,0.6,0.1c0.5,0,0.9-0.1,1.2-0.3c0.3-0.2,0.5-0.4,0.6-0.6c0.1-0.2,0.2-0.5,0.3-0.8
            c0-0.3,0-0.5,0-0.6V117.1z"/>
        <path class="st0" d="M193.7,112v1.8h-2v4.9c0,0.5,0.1,0.8,0.2,0.9c0.2,0.2,0.5,0.2,0.9,0.2c0.2,0,0.3,0,0.4,0c0.1,0,0.3,0,0.4-0.1
            v2.1c-0.2,0-0.5,0.1-0.8,0.1c-0.3,0-0.6,0-0.8,0c-0.4,0-0.8,0-1.2-0.1c-0.4-0.1-0.7-0.2-1-0.3c-0.3-0.2-0.5-0.4-0.7-0.7
            c-0.2-0.3-0.2-0.7-0.2-1.2v-5.8h-1.6V112h1.6v-3h2.7v3H193.7z"/>
        <path class="st0" d="M195.1,110.5v-2.2h2.7v2.2H195.1z M197.8,112v9.9h-2.7V112H197.8z"/>
        <path class="st0" d="M200,114.8c0.2-0.6,0.6-1.2,1-1.6c0.4-0.5,1-0.8,1.6-1c0.6-0.2,1.3-0.4,2.1-0.4c0.8,0,1.5,0.1,2.1,0.4
            c0.6,0.2,1.2,0.6,1.6,1c0.4,0.5,0.8,1,1,1.6c0.2,0.6,0.4,1.4,0.4,2.1c0,0.8-0.1,1.5-0.4,2.1c-0.2,0.6-0.6,1.2-1,1.6
            c-0.4,0.5-1,0.8-1.6,1c-0.6,0.2-1.3,0.4-2.1,0.4c-0.8,0-1.5-0.1-2.1-0.4c-0.6-0.2-1.2-0.6-1.6-1c-0.4-0.5-0.8-1-1-1.6
            c-0.2-0.6-0.4-1.3-0.4-2.1C199.6,116.2,199.8,115.5,200,114.8z M202.5,118.1c0.1,0.4,0.2,0.7,0.4,1c0.2,0.3,0.4,0.5,0.7,0.7
            c0.3,0.2,0.7,0.3,1.1,0.3s0.8-0.1,1.2-0.3c0.3-0.2,0.6-0.4,0.7-0.7c0.2-0.3,0.3-0.6,0.4-1c0.1-0.4,0.1-0.8,0.1-1.2
            c0-0.4,0-0.8-0.1-1.2c-0.1-0.4-0.2-0.7-0.4-1s-0.4-0.5-0.7-0.7c-0.3-0.2-0.7-0.3-1.2-0.3s-0.8,0.1-1.1,0.3
            c-0.3,0.2-0.5,0.4-0.7,0.7c-0.2,0.3-0.3,0.6-0.4,1c-0.1,0.4-0.1,0.8-0.1,1.2C202.3,117.4,202.4,117.7,202.5,118.1z"/>
        <path class="st0" d="M214.2,112v1.4h0.1c0.3-0.6,0.8-1,1.3-1.2c0.5-0.3,1.1-0.4,1.7-0.4c0.7,0,1.3,0.1,1.8,0.3
            c0.5,0.2,0.8,0.5,1.1,0.8c0.3,0.3,0.5,0.8,0.6,1.3c0.1,0.5,0.2,1.1,0.2,1.7v6.1h-2.7v-5.6c0-0.8-0.1-1.4-0.4-1.8
            c-0.3-0.4-0.7-0.6-1.4-0.6c-0.7,0-1.3,0.2-1.6,0.7c-0.3,0.4-0.5,1.2-0.5,2.2v5.2h-2.7V112H214.2z"/>
        <path class="st0" d="M225.2,119.4c0.1,0.2,0.3,0.4,0.5,0.5c0.2,0.1,0.4,0.2,0.7,0.3c0.2,0.1,0.5,0.1,0.8,0.1c0.2,0,0.4,0,0.6-0.1
            c0.2,0,0.4-0.1,0.6-0.2c0.2-0.1,0.3-0.2,0.4-0.4c0.1-0.2,0.2-0.4,0.2-0.6c0-0.4-0.3-0.7-0.8-0.9c-0.5-0.2-1.3-0.4-2.3-0.6
            c-0.4-0.1-0.8-0.2-1.2-0.3c-0.4-0.1-0.7-0.3-1-0.5c-0.3-0.2-0.5-0.4-0.7-0.7c-0.2-0.3-0.3-0.7-0.3-1.1c0-0.6,0.1-1.2,0.4-1.6
            c0.2-0.4,0.6-0.7,1-1c0.4-0.2,0.9-0.4,1.4-0.5c0.5-0.1,1-0.1,1.6-0.1c0.5,0,1.1,0.1,1.6,0.2c0.5,0.1,1,0.3,1.3,0.5
            c0.4,0.2,0.7,0.6,1,1c0.3,0.4,0.4,0.9,0.5,1.5h-2.6c0-0.5-0.2-0.9-0.6-1.1c-0.4-0.2-0.8-0.3-1.3-0.3c-0.2,0-0.3,0-0.5,0
            c-0.2,0-0.3,0.1-0.5,0.1c-0.1,0.1-0.3,0.2-0.4,0.3c-0.1,0.1-0.2,0.3-0.2,0.5c0,0.2,0.1,0.4,0.3,0.6c0.2,0.2,0.4,0.3,0.7,0.4
            c0.3,0.1,0.6,0.2,1,0.3c0.4,0.1,0.7,0.2,1.1,0.2c0.4,0.1,0.8,0.2,1.2,0.3c0.4,0.1,0.7,0.3,1,0.5c0.3,0.2,0.5,0.5,0.7,0.8
            c0.2,0.3,0.3,0.7,0.3,1.2c0,0.6-0.1,1.2-0.4,1.6c-0.3,0.4-0.6,0.8-1,1.1s-0.9,0.5-1.4,0.6c-0.5,0.1-1.1,0.2-1.7,0.2
            c-0.6,0-1.1-0.1-1.7-0.2c-0.5-0.1-1-0.3-1.5-0.6c-0.4-0.3-0.8-0.6-1-1.1c-0.3-0.4-0.4-1-0.4-1.6h2.6
            C225,119,225.1,119.2,225.2,119.4z"/>
        <path class="st0" d="M245.5,108.3l5.1,13.6h-3.1l-1-3h-5.1l-1.1,3h-3l5.2-13.6H245.5z M245.7,116.6l-1.7-5h0l-1.8,5H245.7z"/>
        <path class="st0" d="M254.1,112v1.8h0c0.1-0.3,0.3-0.6,0.5-0.8c0.2-0.3,0.5-0.5,0.7-0.7c0.3-0.2,0.6-0.3,0.9-0.4
            c0.3-0.1,0.6-0.2,1-0.2c0.2,0,0.4,0,0.6,0.1v2.5c-0.1,0-0.3,0-0.5-0.1c-0.2,0-0.4,0-0.5,0c-0.5,0-0.9,0.1-1.3,0.2
            c-0.3,0.2-0.6,0.4-0.8,0.7c-0.2,0.3-0.4,0.6-0.4,1c-0.1,0.4-0.1,0.8-0.1,1.2v4.4h-2.7V112H254.1z"/>
        <path class="st0" d="M261.5,119.5c0.4,0.4,1,0.6,1.8,0.6c0.5,0,1-0.1,1.4-0.4c0.4-0.3,0.6-0.6,0.7-0.9h2.4c-0.4,1.2-1,2-1.8,2.5
            c-0.8,0.5-1.7,0.8-2.9,0.8c-0.8,0-1.5-0.1-2.1-0.4c-0.6-0.2-1.2-0.6-1.6-1.1c-0.4-0.5-0.8-1-1-1.6c-0.2-0.6-0.4-1.3-0.4-2.1
            c0-0.7,0.1-1.4,0.4-2.1c0.2-0.6,0.6-1.2,1-1.6c0.4-0.5,1-0.8,1.6-1.1c0.6-0.3,1.3-0.4,2.1-0.4c0.8,0,1.6,0.2,2.2,0.5
            c0.6,0.3,1.1,0.8,1.5,1.3c0.4,0.5,0.7,1.2,0.9,1.9c0.2,0.7,0.2,1.4,0.2,2.2h-7.1C260.9,118.5,261.1,119.1,261.5,119.5z
             M264.6,114.3c-0.3-0.4-0.8-0.5-1.5-0.5c-0.4,0-0.8,0.1-1.1,0.2c-0.3,0.1-0.5,0.3-0.7,0.5c-0.2,0.2-0.3,0.4-0.4,0.7
            c-0.1,0.2-0.1,0.5-0.1,0.6h4.4C265.1,115.2,264.9,114.7,264.6,114.3z"/>
        <path class="st0" d="M281.6,108.3c0.6,0,1.2,0.1,1.8,0.2s1,0.3,1.4,0.6c0.4,0.3,0.7,0.6,0.9,1c0.2,0.4,0.3,1,0.3,1.6
            c0,0.7-0.2,1.3-0.5,1.7c-0.3,0.5-0.8,0.8-1.4,1.1c0.8,0.2,1.5,0.7,1.9,1.3c0.4,0.6,0.6,1.3,0.6,2.2c0,0.7-0.1,1.3-0.4,1.8
            c-0.3,0.5-0.6,0.9-1.1,1.2c-0.5,0.3-1,0.6-1.5,0.7c-0.6,0.2-1.2,0.2-1.8,0.2h-6.6v-13.6H281.6z M281.2,113.8c0.5,0,1-0.1,1.3-0.4
            c0.3-0.3,0.5-0.7,0.5-1.2c0-0.3-0.1-0.6-0.2-0.8c-0.1-0.2-0.3-0.4-0.5-0.5c-0.2-0.1-0.4-0.2-0.7-0.2c-0.2,0-0.5-0.1-0.8-0.1h-2.8
            v3.2H281.2z M281.4,119.6c0.3,0,0.6,0,0.8-0.1c0.3-0.1,0.5-0.2,0.7-0.3c0.2-0.1,0.4-0.3,0.5-0.5c0.1-0.2,0.2-0.5,0.2-0.9
            c0-0.7-0.2-1.2-0.6-1.5c-0.4-0.3-0.9-0.4-1.6-0.4h-3.3v3.7H281.4z"/>
        <path class="st0" d="M291.2,119.5c0.4,0.4,1,0.6,1.8,0.6c0.5,0,1-0.1,1.4-0.4c0.4-0.3,0.6-0.6,0.7-0.9h2.4c-0.4,1.2-1,2-1.8,2.5
            c-0.8,0.5-1.7,0.8-2.9,0.8c-0.8,0-1.5-0.1-2.1-0.4c-0.6-0.2-1.2-0.6-1.6-1.1c-0.4-0.5-0.8-1-1-1.6c-0.2-0.6-0.4-1.3-0.4-2.1
            c0-0.7,0.1-1.4,0.4-2.1c0.2-0.6,0.6-1.2,1-1.6c0.4-0.5,1-0.8,1.6-1.1c0.6-0.3,1.3-0.4,2.1-0.4c0.8,0,1.6,0.2,2.2,0.5
            c0.6,0.3,1.1,0.8,1.5,1.3c0.4,0.5,0.7,1.2,0.9,1.9c0.2,0.7,0.2,1.4,0.2,2.2h-7.1C290.6,118.5,290.8,119.1,291.2,119.5z
             M294.3,114.3c-0.3-0.4-0.8-0.5-1.5-0.5c-0.4,0-0.8,0.1-1.1,0.2c-0.3,0.1-0.5,0.3-0.7,0.5c-0.2,0.2-0.3,0.4-0.4,0.7
            c-0.1,0.2-0.1,0.5-0.1,0.6h4.4C294.8,115.2,294.6,114.7,294.3,114.3z"/>
        <path class="st0" d="M304.7,112v1.8h-2v4.9c0,0.5,0.1,0.8,0.2,0.9s0.5,0.2,0.9,0.2c0.2,0,0.3,0,0.4,0c0.1,0,0.3,0,0.4-0.1v2.1
            c-0.2,0-0.5,0.1-0.8,0.1c-0.3,0-0.6,0-0.8,0c-0.4,0-0.8,0-1.2-0.1c-0.4-0.1-0.7-0.2-1-0.3c-0.3-0.2-0.5-0.4-0.7-0.7
            c-0.2-0.3-0.2-0.7-0.2-1.2v-5.8h-1.6V112h1.6v-3h2.7v3H304.7z"/>
        <path class="st0" d="M311.4,112v1.8h-2v4.9c0,0.5,0.1,0.8,0.2,0.9c0.2,0.2,0.5,0.2,0.9,0.2c0.2,0,0.3,0,0.4,0c0.1,0,0.3,0,0.4-0.1
            v2.1c-0.2,0-0.5,0.1-0.8,0.1c-0.3,0-0.6,0-0.8,0c-0.4,0-0.8,0-1.2-0.1c-0.4-0.1-0.7-0.2-1-0.3c-0.3-0.2-0.5-0.4-0.7-0.7
            c-0.2-0.3-0.2-0.7-0.2-1.2v-5.8h-1.6V112h1.6v-3h2.7v3H311.4z"/>
        <path class="st0" d="M315.6,119.5c0.4,0.4,1,0.6,1.8,0.6c0.5,0,1-0.1,1.4-0.4c0.4-0.3,0.6-0.6,0.7-0.9h2.4c-0.4,1.2-1,2-1.8,2.5
            c-0.8,0.5-1.7,0.8-2.9,0.8c-0.8,0-1.5-0.1-2.1-0.4c-0.6-0.2-1.2-0.6-1.6-1.1c-0.4-0.5-0.8-1-1-1.6c-0.2-0.6-0.4-1.3-0.4-2.1
            c0-0.7,0.1-1.4,0.4-2.1c0.2-0.6,0.6-1.2,1-1.6c0.4-0.5,1-0.8,1.6-1.1c0.6-0.3,1.3-0.4,2.1-0.4c0.8,0,1.6,0.2,2.2,0.5
            c0.6,0.3,1.1,0.8,1.5,1.3c0.4,0.5,0.7,1.2,0.9,1.9c0.2,0.7,0.2,1.4,0.2,2.2h-7.1C315,118.5,315.2,119.1,315.6,119.5z M318.6,114.3
            c-0.3-0.4-0.8-0.5-1.5-0.5c-0.4,0-0.8,0.1-1.1,0.2c-0.3,0.1-0.5,0.3-0.7,0.5c-0.2,0.2-0.3,0.4-0.4,0.7c-0.1,0.2-0.1,0.5-0.1,0.6
            h4.4C319.2,115.2,319,114.7,318.6,114.3z"/>
        <path class="st0" d="M326.2,112v1.8h0c0.1-0.3,0.3-0.6,0.5-0.8c0.2-0.3,0.5-0.5,0.7-0.7c0.3-0.2,0.6-0.3,0.9-0.4
            c0.3-0.1,0.6-0.2,1-0.2c0.2,0,0.4,0,0.6,0.1v2.5c-0.1,0-0.3,0-0.5-0.1s-0.4,0-0.5,0c-0.5,0-0.9,0.1-1.3,0.2
            c-0.3,0.2-0.6,0.4-0.8,0.7c-0.2,0.3-0.4,0.6-0.4,1c-0.1,0.4-0.1,0.8-0.1,1.2v4.4h-2.7V112H326.2z"/>
        <path class="st0" d="M335.6,110.8v-2.5h11.2v2.5h-4.1v11.1h-3v-11.1H335.6z"/>
        <path class="st0" d="M346,114.8c0.2-0.6,0.6-1.2,1-1.6c0.4-0.5,1-0.8,1.6-1c0.6-0.2,1.3-0.4,2.1-0.4c0.8,0,1.5,0.1,2.1,0.4
            s1.2,0.6,1.6,1c0.4,0.5,0.8,1,1,1.6c0.2,0.6,0.4,1.4,0.4,2.1c0,0.8-0.1,1.5-0.4,2.1c-0.2,0.6-0.6,1.2-1,1.6c-0.4,0.5-1,0.8-1.6,1
            s-1.3,0.4-2.1,0.4c-0.8,0-1.5-0.1-2.1-0.4c-0.6-0.2-1.2-0.6-1.6-1c-0.4-0.5-0.8-1-1-1.6c-0.2-0.6-0.4-1.3-0.4-2.1
            C345.6,116.2,345.7,115.5,346,114.8z M348.4,118.1c0.1,0.4,0.2,0.7,0.4,1c0.2,0.3,0.4,0.5,0.7,0.7c0.3,0.2,0.7,0.3,1.1,0.3
            s0.8-0.1,1.2-0.3c0.3-0.2,0.6-0.4,0.7-0.7c0.2-0.3,0.3-0.6,0.4-1c0.1-0.4,0.1-0.8,0.1-1.2c0-0.4,0-0.8-0.1-1.2
            c-0.1-0.4-0.2-0.7-0.4-1s-0.4-0.5-0.7-0.7c-0.3-0.2-0.7-0.3-1.2-0.3s-0.8,0.1-1.1,0.3c-0.3,0.2-0.5,0.4-0.7,0.7
            c-0.2,0.3-0.3,0.6-0.4,1c-0.1,0.4-0.1,0.8-0.1,1.2C348.3,117.4,348.3,117.7,348.4,118.1z"/>
        <path class="st0" d="M366.9,122.6c-0.1,0.5-0.3,1-0.7,1.4c-0.4,0.4-0.9,0.8-1.5,1.1c-0.7,0.3-1.6,0.5-2.7,0.5
            c-0.5,0-1-0.1-1.5-0.2c-0.5-0.1-1-0.3-1.4-0.6c-0.4-0.3-0.8-0.6-1-1c-0.3-0.4-0.4-0.9-0.5-1.4h2.7c0.1,0.5,0.4,0.9,0.7,1.1
            c0.4,0.2,0.8,0.3,1.3,0.3c0.8,0,1.3-0.2,1.7-0.7c0.3-0.5,0.5-1,0.5-1.7v-1.3h0c-0.3,0.5-0.7,0.9-1.3,1.2c-0.5,0.2-1.1,0.4-1.7,0.4
            c-0.7,0-1.4-0.1-1.9-0.4c-0.5-0.3-1-0.6-1.3-1.1c-0.3-0.5-0.6-1-0.8-1.6c-0.2-0.6-0.2-1.2-0.2-1.9c0-0.6,0.1-1.3,0.3-1.8
            s0.5-1.1,0.8-1.6c0.4-0.5,0.8-0.8,1.3-1.1c0.5-0.3,1.1-0.4,1.8-0.4c0.6,0,1.2,0.1,1.7,0.4c0.5,0.2,0.9,0.6,1.2,1.2h0V112h2.6v9.2
            C367,121.6,367,122.1,366.9,122.6z M363.2,119.3c0.3-0.2,0.5-0.4,0.7-0.6c0.2-0.3,0.3-0.5,0.4-0.9c0.1-0.3,0.1-0.7,0.1-1
            c0-0.4,0-0.8-0.1-1.2s-0.2-0.7-0.4-1c-0.2-0.3-0.4-0.5-0.7-0.7c-0.3-0.2-0.7-0.2-1.1-0.2c-0.4,0-0.7,0.1-1,0.2
            c-0.3,0.2-0.5,0.4-0.7,0.6c-0.2,0.3-0.3,0.6-0.4,0.9c-0.1,0.3-0.1,0.7-0.1,1.1c0,0.4,0,0.7,0.1,1.1c0.1,0.4,0.2,0.7,0.4,0.9
            c0.2,0.3,0.4,0.5,0.7,0.7c0.3,0.2,0.6,0.3,1,0.3C362.5,119.6,362.9,119.5,363.2,119.3z"/>
        <path class="st0" d="M372.1,119.5c0.4,0.4,1,0.6,1.8,0.6c0.5,0,1-0.1,1.4-0.4c0.4-0.3,0.6-0.6,0.7-0.9h2.4c-0.4,1.2-1,2-1.8,2.5
            c-0.8,0.5-1.7,0.8-2.9,0.8c-0.8,0-1.5-0.1-2.1-0.4c-0.6-0.2-1.2-0.6-1.6-1.1c-0.4-0.5-0.8-1-1-1.6c-0.2-0.6-0.4-1.3-0.4-2.1
            c0-0.7,0.1-1.4,0.4-2.1c0.2-0.6,0.6-1.2,1-1.6c0.4-0.5,1-0.8,1.6-1.1c0.6-0.3,1.3-0.4,2.1-0.4c0.8,0,1.6,0.2,2.2,0.5
            c0.6,0.3,1.1,0.8,1.5,1.3c0.4,0.5,0.7,1.2,0.9,1.9c0.2,0.7,0.2,1.4,0.2,2.2h-7.1C371.5,118.5,371.7,119.1,372.1,119.5z
             M375.2,114.3c-0.3-0.4-0.8-0.5-1.5-0.5c-0.4,0-0.8,0.1-1.1,0.2c-0.3,0.1-0.5,0.3-0.7,0.5c-0.2,0.2-0.3,0.4-0.4,0.7
            c-0.1,0.2-0.1,0.5-0.1,0.6h4.4C375.7,115.2,375.5,114.7,375.2,114.3z"/>
        <path class="st0" d="M385.6,112v1.8h-2v4.9c0,0.5,0.1,0.8,0.2,0.9s0.5,0.2,0.9,0.2c0.2,0,0.3,0,0.4,0c0.1,0,0.3,0,0.4-0.1v2.1
            c-0.2,0-0.5,0.1-0.8,0.1c-0.3,0-0.6,0-0.8,0c-0.4,0-0.8,0-1.2-0.1c-0.4-0.1-0.7-0.2-1-0.3c-0.3-0.2-0.5-0.4-0.7-0.7
            c-0.2-0.3-0.2-0.7-0.2-1.2v-5.8h-1.6V112h1.6v-3h2.7v3H385.6z"/>
        <path class="st0" d="M389.6,108.3v5.1h0.1c0.3-0.6,0.8-1,1.3-1.2c0.5-0.3,1.1-0.4,1.6-0.4c0.7,0,1.3,0.1,1.8,0.3
            c0.5,0.2,0.8,0.5,1.1,0.8c0.3,0.3,0.5,0.8,0.6,1.3c0.1,0.5,0.2,1.1,0.2,1.7v6.1h-2.7v-5.6c0-0.8-0.1-1.4-0.4-1.8
            c-0.3-0.4-0.7-0.6-1.4-0.6c-0.7,0-1.3,0.2-1.6,0.7c-0.3,0.4-0.5,1.2-0.5,2.2v5.2h-2.7v-13.6H389.6z"/>
        <path class="st0" d="M401.1,119.5c0.4,0.4,1,0.6,1.8,0.6c0.5,0,1-0.1,1.4-0.4c0.4-0.3,0.6-0.6,0.7-0.9h2.4c-0.4,1.2-1,2-1.8,2.5
            c-0.8,0.5-1.7,0.8-2.9,0.8c-0.8,0-1.5-0.1-2.1-0.4c-0.6-0.2-1.2-0.6-1.6-1.1c-0.4-0.5-0.8-1-1-1.6c-0.2-0.6-0.4-1.3-0.4-2.1
            c0-0.7,0.1-1.4,0.4-2.1c0.2-0.6,0.6-1.2,1-1.6c0.4-0.5,1-0.8,1.6-1.1c0.6-0.3,1.3-0.4,2.1-0.4c0.8,0,1.6,0.2,2.2,0.5
            c0.6,0.3,1.1,0.8,1.5,1.3c0.4,0.5,0.7,1.2,0.9,1.9c0.2,0.7,0.2,1.4,0.2,2.2h-7.1C400.5,118.5,400.7,119.1,401.1,119.5z
             M404.2,114.3c-0.3-0.4-0.8-0.5-1.5-0.5c-0.4,0-0.8,0.1-1.1,0.2c-0.3,0.1-0.5,0.3-0.7,0.5c-0.2,0.2-0.3,0.4-0.4,0.7
            c-0.1,0.2-0.1,0.5-0.1,0.6h4.4C404.7,115.2,404.5,114.7,404.2,114.3z"/>
        <path class="st0" d="M411.7,112v1.8h0c0.1-0.3,0.3-0.6,0.5-0.8c0.2-0.3,0.5-0.5,0.7-0.7c0.3-0.2,0.6-0.3,0.9-0.4
            c0.3-0.1,0.6-0.2,1-0.2c0.2,0,0.4,0,0.6,0.1v2.5c-0.1,0-0.3,0-0.5-0.1c-0.2,0-0.4,0-0.5,0c-0.5,0-0.9,0.1-1.3,0.2
            c-0.3,0.2-0.6,0.4-0.8,0.7c-0.2,0.3-0.4,0.6-0.4,1c-0.1,0.4-0.1,0.8-0.1,1.2v4.4h-2.7V112H411.7z"/>
    </g>)raw";

    static const char* svg2 = R"raw(
<g>
        <linearGradient id="SVGID_1_" gradientUnits="userSpaceOnUse" x1="30.5941" y1="54.4356" x2="30.5941" y2="127.221">
            <stop  offset="0.1127" style="stop-color:#235075"/>
            <stop  offset="0.7537" style="stop-color:#0C96D0"/>
        </linearGradient>
        <path class="st1" d="M46.8,86.8L46.8,86.8c-17.9,0-32.3-14.5-32.3-32.3v72.8h0l0,0v0h32.3V95.7h0c0-0.3,0-0.5,0-0.8V86.8z"/>

            <linearGradient id="SVGID_2_" gradientUnits="userSpaceOnUse" x1="-239.4459" y1="105.6518" x2="-174.7478" y2="105.6518" gradientTransform="matrix(-1 0 0 -1 -128 176.2619)">
            <stop  offset="0.197" style="stop-color:#144A71"/>
            <stop  offset="0.3768" style="stop-color:#ADC5D1"/>
            <stop  offset="0.6305" style="stop-color:#0C96D0"/>
            <stop  offset="0.8873" style="stop-color:#235075"/>
        </linearGradient>
        <path class="st2" d="M46.7,54.4c0,17.9,14.5,32.3,32.3,32.3C79.1,68.9,64.6,54.4,46.7,54.4z"/>
        <linearGradient id="SVGID_3_" gradientUnits="userSpaceOnUse" x1="95.2714" y1="86.7846" x2="95.2714" y2="13.9992">
            <stop  offset="0.1127" style="stop-color:#235075"/>
            <stop  offset="0.7537" style="stop-color:#0C96D0"/>
        </linearGradient>
        <path class="st3" d="M111.4,14H79.1v31.5h0c0,0.3,0,0.5,0,0.8v8.1h0c12,0,22.4,6.5,28,16.2c2.8,4.8,4.3,10.3,4.3,16.2L111.4,14
            L111.4,14L111.4,14z"/>
        <linearGradient id="SVGID_4_" gradientUnits="userSpaceOnUse" x1="21.8517" y1="75.288" x2="64.9416" y2="32.1981">
            <stop  offset="0.2463" style="stop-color:#0C96D0"/>
            <stop  offset="0.8873" style="stop-color:#235075"/>
        </linearGradient>
        <path class="st4" d="M46.7,54.4v-8.1c0-0.3,0-0.5,0-0.8h0V14H14.4v0l0,0h0v40.4v0c0,17.9,14.5,32.3,32.3,32.3h0h32.3
            C61.2,86.8,46.7,72.3,46.7,54.4z"/>
        <linearGradient id="SVGID_5_" gradientUnits="userSpaceOnUse" x1="48.0744" y1="53.109" x2="116.8162" y2="121.8507">
            <stop  offset="0.1127" style="stop-color:#235075"/>
            <stop  offset="0.7537" style="stop-color:#0C96D0"/>
        </linearGradient>
        <path class="st5" d="M107.1,70.6c-5.6-9.7-16-16.2-28-16.2h0H46.7c17.9,0,32.3,14.5,32.3,32.3v8.1c0,0.3,0,0.5,0,0.8h0v31.5h32.3
            v0l0,0h0V86.8v0C111.4,80.9,109.9,75.4,107.1,70.6z"/>
    </g>
    <g>
        <path class="st6" d="M174.6,59.9h-32v21.7h37.5V95h-54.2V14H180v13.5h-37.4v19.3h32V59.9z"/>
        <path class="st6" d="M206.5,81.6h35.4V95h-52V14h16.7V81.6z"/>
        <path class="st6" d="M269.2,95h-16.7V14h16.7V95z"/>
        <path class="st6" d="M348,68c-0.6,8.7-3.8,15.6-9.6,20.6c-5.8,5-13.4,7.5-22.9,7.5c-10.4,0-18.5-3.5-24.5-10.5
            c-5.9-7-8.9-16.6-8.9-28.8v-4.9c0-7.8,1.4-14.6,4.1-20.6c2.7-5.9,6.7-10.5,11.7-13.7c5.1-3.2,11-4.8,17.7-4.8
            c9.3,0,16.8,2.5,22.5,7.5c5.7,5,9,12,9.9,21.1h-16.7c-0.4-5.2-1.9-9-4.4-11.4s-6.3-3.5-11.4-3.5c-5.6,0-9.7,2-12.5,6
            s-4.2,10.2-4.2,18.5v6.1c0,8.7,1.3,15.1,4,19.2c2.6,4,6.8,6.1,12.5,6.1c5.1,0,9-1.2,11.5-3.5c2.5-2.4,4-6,4.4-10.9H348z"/>
        <path class="st6" d="M400.9,73.7c0-3.2-1.1-5.6-3.3-7.3c-2.2-1.7-6.2-3.5-12-5.3c-5.8-1.9-10.3-3.7-13.7-5.5
            c-9.2-5-13.8-11.7-13.8-20.1c0-4.4,1.2-8.3,3.7-11.7c2.5-3.4,6-6.1,10.6-8s9.8-2.9,15.5-2.9c5.8,0,10.9,1,15.4,3.1
            c4.5,2.1,8,5.1,10.5,8.9c2.5,3.8,3.7,8.2,3.7,13h-16.7c0-3.7-1.2-6.6-3.5-8.6c-2.3-2.1-5.6-3.1-9.8-3.1c-4.1,0-7.2,0.9-9.5,2.6
            c-2.3,1.7-3.4,4-3.4,6.8c0,2.6,1.3,4.8,4,6.6c2.6,1.8,6.5,3.4,11.7,5c9.5,2.9,16.4,6.4,20.7,10.6c4.3,4.2,6.5,9.5,6.5,15.8
            c0,7-2.6,12.5-7.9,16.5c-5.3,4-12.4,6-21.4,6c-6.2,0-11.9-1.1-17-3.4c-5.1-2.3-9-5.4-11.7-9.4c-2.7-4-4-8.6-4-13.8h16.7
            c0,8.9,5.3,13.4,16,13.4c4,0,7.1-0.8,9.3-2.4C399.8,78.9,400.9,76.6,400.9,73.7z"/>
    </g>
</g>
</svg>)raw";

    static const char* indexPage3 = R"raw(<br>
    <h1>Web server interface</h1>
    <h2>
      <a href="https://github.com/GMLC-TDC/HELICS">HELICS Source repository</a><br>
      <a href="https://docs.helics.org/en/latest/index.html"
        >HELICS Documentation</a
      ><br>

<a href="https://docs.helics.org/en/latest/user-guide/webserver.html"
        >Using the web server</a
      ><br>

<a href="https://docs.helics.org/en/latest/user-guide/queries.html"
        >Available queries</a
      ><br>
    </h2>
  </body>
</html>
)raw";

}  // namespace webserver
}  // namespace helics
