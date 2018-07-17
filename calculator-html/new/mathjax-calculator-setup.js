/* -*- Mode: Javascript; indent-tabs-mode:nil; js-indent-level: 2 -*- */
/* vim: set ts =2 et sw =2 tw =80: */

/*************************************************************
 *
 *  MathJax/config/local/local.js
 *
 *  Include changes and configuration local to your installation
 *  in this file.  For example, common macros can be defined here
 *  (see below).  To use this file, add "local/local.js" to the
 *  config array in MathJax.js or your MathJax.Hub.Config() call.
 *
 *  ---------------------------------------------------------------------
 *
 *  Copyright (c) 2009-2015 The MathJax Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

function selectElementContents(el) { 
  var range = document.createRange();
  range.selectNodeContents(el);
  var sel = window.getSelection();
  sel.removeAllRanges();
  sel.addRange(range);
  el.focus();
}

function showTex(originalTex, item, event) { 
  if (item['calculatorTexShown'] === undefined) {
    item['calculatorTexShown'] = false;
  }
  item['calculatorTexShown'] = !item['calculatorTexShown'];
  if (!item['calculatorTexShown']) { 
    item.parentNode.removeChild(item.nextSibling);
    return;
  }
  var flaSpan = document.createElement('span');
  if (originalTex.startsWith("\\displaystyle ")) {
    originalTex = originalTex.slice(14);
  }
  flaSpan.textContent = originalTex;
  item.parentNode.insertBefore(flaSpan, item.nextSibling);
  item.blur();
  selectElementContents(flaSpan);
  //MathJax.Menu.ShowSource.Text(originalTex||"No Original Source to Show", event);
}

function configureMathJaxForCalculator() { 
  //mathjax configuration comes before loading the mathjax script, as requested by the documentation.
  MathJax.Hub.Config({
    extensions: ["tex2jax.js"],
    jax: ["input/TeX", "output/HTML-CSS"],
    tex2jax: {
      inlineMath: [ ['\\(','\\)'] ],
      displayMath: [ ['\\[','\\]'] ],
      processClass: "calculator|calculatorStudentAnswer|calculatorAnswer",
      processEscapes: true
    },
    "HTML-CSS": { availableFonts: ["TeX"] },
    TeX: {
      Macros: {
        diff: "{\\text {d}}",
      },
      MAXBUFFER: 100000
    },
    jsMath2jax: {
      preview: "TeX"
    },
    });

  MathJax.Hub.Register.StartupHook("MathEvents Ready", function () {
    var HUB = MathJax.Hub, MENUSETTINGS = HUB.config.menuSettings;
    var EVENT = MathJax.Extension.MathEvents.Event;
    var MENU = MathJax.Menu;
    //
    //  Override the double-click function with our own.
    //  It calls on the MathJax.Menu code, so make sure that is loaded and
    //  if not, load it and call the double-click handler again afterward.
    //
    EVENT.DblClick = function(event) { 
      if (MENU) { 
        var jax = HUB.getJaxFor(this);
        showTex(jax.originalText, this, event);
      } else { 
        MathJax.Callback.Queue(["Require", MathJax.Ajax, "[MathJax]/extensions/MathMenu.js"], ["DblClick", this, {}]);
      }
      return EVENT.False(event);
    }
    //
    //  Make sure we don't have double-click zoom menu item selected
    //  and disable the zoom trigger menu item.
    //  Also set the MENU item so that it will be available above.
    //
    if (MENUSETTINGS.zoom === "Double-Click") { 
      MENUSETTINGS.zoom = "None";
    }
    MathJax.Hub.Register.StartupHook("MathMenu Ready", function () {
      MENU = MathJax.Menu;
      MENU.menu.Find("Math Settings", "Zoom Trigger","Double-Click").disabled = true;
    });
  });


  /****************************************************
   *
   *  forminput.js
   *
   *  Implements the \FormInput extension for including text <input> elements in
   *  math expressions.  This only works in HTML-CSS output (all browsers),
   *  and NativeMML output in Firefox (but not IE/MathPlayer or Opera).
   *
   *  The macro is \FormInput{name} where "name" is the CSS id to use for the
   *  <input> element.  The Default size is 2, but you can use an optional
   *  argument to set a different size, as in \FormInput[size]{name}.  Finally,
   *  you can use a second optional parameter to specify a CSS class for the
   *  element (which can be styled to suit your needs).  All the elements
   *  generated by this extension are in class MathJax_Input, so you can
   *  style that to change the default presentation.
   *
   *  You can load this file via the config= file parameter on the script
   *  tag that loads MathJax.js, or by including it in the extensions
   *  array in your configuration.
   *
   *  Be sure to change the loadComplete() address to the URL
   *  of the location of this file on your server.
   */
  MathJax.Callback.Queue(
  MathJax.Hub.Register.StartupHook("TeX Jax Ready", function() {
    var VERSION = "1.0";

    var TEX = MathJax.InputJax.TeX,
        TEXDEF = TEX.Definitions,
        MML = MathJax.ElementJax.mml,
        HTML = MathJax.HTML;

    TEXDEF.macros.FormInput = "FormInput";

    TEX.Parse.Augment({
      //
      //  Implements \FormInput[value][size][class]{name}
      //
      FormInput: function (name) {
        var val = this.GetBrackets(name);
        var size = this.GetBrackets(name);
        var cls = this.GetBrackets(name);
        var inputName = this.GetArgument(name);
        //if (window.mathjaxIdCounter === undefined) {
        //  window.mathjaxIdCounter = 0;
        //}
        //window.mathjaxIdCounter ++;
//        var id = `mathjax_id_${window.mathjaxIdCounter}_${inputName}`;
//        console.log("DEBUG: mathjax id is: " + id);
        //alert("name: " + name + " size: "+ size + " cls: " + cls + " val: " + val + " id: "+ id);
        if (size === null || size === "" || size === undefined) {
          size = "2";
        }
        if (val === null || val === undefined || val === "") {
          val = "";
        }
        cls = ("MathJax_Input " + (cls || "")).replace(/ +$/,"");
        var input = HTML.Element("input", {
          type: "text", 
          name: inputName, 
//          id: id, 
          size: size, 
          className: cls, 
          value: val
        });
        input.setAttribute("xmlns","http://www.w3.org/1999/xhtml");
        var mml = MML["annotation-xml"](MML.xml(input)).With({
          encoding:"application/xhtml+xml", isToken:true
        });
        this.Push(MML.semantics(mml));
      }
    });

  }));


  //MathJax.Hub.Register.StartupHook("TeX Jax Ready",function () {
  //  var TEX = MathJax.InputJax.TeX;

    // place macros here.  E.g.:
    //   TEX.Macro("R","{\\bf R}");
    //   TEX.Macro("op","\\mathop{\\rm #1}",1); // a macro with 1 parameter

  //});

  MathJax.Ajax.loadComplete("mathjax-calculator-setup.js");
}

var waitingForMathJaxCounter = 0;

function waitForMathJax() { 
  waitingForMathJaxCounter ++;
  if (waitingForMathJaxCounter > 100) {
    return;
  }
  if (MathJax === undefined) { 
    setTimeout(waitForMathJax, 100);
    return;
  }
  configureMathJaxForCalculator();
//  setTimeout(ConfigureMathJaxForCalculator,2000);
}
//waitForMathJax();
configureMathJaxForCalculator();
