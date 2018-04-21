"use strict";

function updateDatabasePageCallback(incoming, output){
  try {
    var theParsed = JSON.parse(incoming);
    console.log(JSON.stringify(theParsed));
    var theOutput = document.getElementById("divDatabaseOutput");
    if ("rows" in theParsed) {
      theOutput.innerHTML = getHtmlFromArrayOfObjects(theParsed.rows);
    } else {
      for (var counterCollection = 0; counterCollection < theParsed.collections.length; counterCollection ++){
        var currentCollection = theParsed.collections[counterCollection]; 
        theParsed.collections[counterCollection] = `
        <a href = "#" onclick =
        "thePage.pages.database.storage.currentTable = '${currentCollection}'; updateDatabasePage();">${currentCollection}</a>`;
      }
      theOutput.innerHTML = getHtmlFromArrayOfObjects(theParsed.collections);
    }
  } catch (e) {
    console.log(`Error parsing calculator output. ${e}`);
  }
}

function updateDatabasePageResetCurrentTable(){
  thePage.pages.database.storage.currentTable = "";
  updateDatabasePage();
}

function updateDatabasePage(){
  thePage.storeSettingsToLocalStorage();
  var theUrl = `${pathnames.calculator}?${pathnames.requestDatabase}&${pathnames.databaseTable}=${thePage.pages.database.storage.currentTable}`;
  submitGET({
    url: theUrl,
    callback: updateDatabasePageCallback
  });
}