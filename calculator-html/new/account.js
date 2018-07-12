"use strict";

function submitChangePassRequestCallback(result, outputComponent) {
  outputComponent = document.getElementById("spanVerification").innerHTML = result;
  document.getElementById("inputPassword").value = document.getElementById("inputNewPasswordInAccount").value;
  document.getElementById("inputOldPasswordInAccount").value = "";
  document.getElementById("inputNewPasswordInAccount").value = "";
  document.getElementById("inputReenteredPasswordInAccount").value = "";
  loginCalculator();
}

function submitChangePassRequest() {
  var inputOldPassword = document.getElementById("inputOldPasswordInAccount");
  var inputNewPassword = document.getElementById("inputNewPasswordInAccount");
  var inputReenteredPassword = document.getElementById("inputReenteredPasswordInAccount");
  var inputEmail = document.getElementById("inputEmail");
  var theURL = "";
  theURL += `${pathnames.calculatorAPI}?request=changePassword&`;
  theURL += `password=${encodeURIComponent(inputOldPassword.value)}&`;
  theURL += `newPassword=${encodeURIComponent(inputNewPassword.value)}&`;
  theURL += `reenteredPassword=${encodeURIComponent(inputReenteredPassword.value)}&`;
  theURL += `email=${encodeURIComponent(inputEmail.value)}&`;
  theURL += "doReload=false&"
  submitGET({
    url: theURL,
    callback: submitChangePassRequestCallback,
    progress: "spanProgressReportGeneral"
  });
}

function updateAccountPage() {
  var usernameInput = document.getElementById("spanUserIdInAccountsPage");
  var emailSpan = document.getElementById(idDOMElements.spanOldEmail);
  usernameInput.innerHTML = thePage.storage.user.name.getValue();
  emailSpan.innerHTML = thePage.storage.user.email.getValue();
}