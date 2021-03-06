"use strict";

var staticWordCompleter = {
  getCompletions: function(editor, session, pos, prefix, callback) {
    callback(null, thePage.aceEditorAutoCompletionWordList.map(function(word) {
      return {
        caption: word,
        value: word,
        meta: "static"
      };
    }));
  }
}

function ctrlSPressAceEditorHandler(event) {
  if (event.ctrlKey !== true) {
    return;
  }
  if (event.keyCode !== 83) {
    return;
  }
  console.log("not implemented yet");
  event.preventDefault();
  submitStringAsMainInput(editor.getValue(), 'spanSubmitEditPageReport', 'modifyPage', null, 'spanSubmitEditPageReport');
}

function storeEditedPage() {
  var editor = thePage.pages.editPage.editor;
  submitStringAsMainInput(editor.getValue(), 'spanSubmitEditPageReport', 'modifyPage', null, 'spanSubmitEditPageReport');
}

function selectEditPageCallback(input, outputComponent) {
  try {
    var parsedInput = JSON.parse(input);
    //document.getElementById('divEditorAce').textContent = decodeURIComponent(parsedInput.content);
    ace.require("ace/ext/language_tools");
    if (thePage.pages.editPage.editor === null) {
      thePage.pages.editPage.editor = ace.edit("divEditorAce");
    }
    var editor = thePage.pages.editPage.editor;
    editor.$blockScrolling = Infinity;
    editor.getSession().setValue(decodeURIComponent(parsedInput.content));
    editor.setTheme("ace/theme/chrome");
    editor.getSession().setMode("ace/mode/xml");
    editor.setOptions({
      enableBasicAutocompletion: true,
      enableLiveAutocompletion: true
    });
    editor.completers = [staticWordCompleter];    
    editor.$blockScrolling = Infinity;
  } catch (e) {
    console.log("Error: " + e)
  }
}

function selectEditPage(currentlyEditedPage) {
  console.log(`DEBUG: Currently edited page: ${currentlyEditedPage}`);
  if (currentlyEditedPage === undefined || currentlyEditedPage === null) { 
    currentlyEditedPage = thePage.storage.editor.currentlyEditedPage.getValue();
  }
  if (currentlyEditedPage === undefined || currentlyEditedPage === null) { 
    currentlyEditedPage = "/coursesavailable/default.txt";
  }
  thePage.storage.editor.currentlyEditedPage.setAndStore(currentlyEditedPage);
  if (thePage.storage.currentPage.getValue() !== thePage.pages.editPage.name) {
    thePage.selectPage(thePage.pages.editPage.name);
    return;
  }
  var theTopicTextArea = document.getElementById("textareaTopicListEntry");
  theTopicTextArea.value = `Title: ${currentlyEditedPage}\nProblem: ${currentlyEditedPage}`;
  theTopicTextArea.cols = currentlyEditedPage.length + 15;

  var theURL = `${pathnames.calculatorAPI}?${pathnames.requestEditPage}&`;
  theURL += `fileName=${thePage.storage.editor.currentlyEditedPage.getValue()}`;
  submitGET({
    url: theURL,
    callback: selectEditPageCallback
  });
}