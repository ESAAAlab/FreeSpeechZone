/*

Free Speech Zone
Wifi-Connected Thermal Printer, censoring everything people say.
Copyright (c) 2016 Kamel Makhloufi. All rights reserved.

This project is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This project is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

---------------

Change your MQTT server credentials, then  use browserify to minimize 
and package into expression.min.js

browserify expression.js | uglifyjs > expression.min.js

*/

(function () {
  'use strict';

  var mqtt = require('mqtt');
  var iconv = require('iconv-lite');
  var $ = require('jquery');
  var validate = require('validate.js');
  
  var client = mqtt.connect('');
  var isConnected = false;

  var defaultMessage = "Enter your message";
  $('#message').val(defaultMessage);
  $('#message').addClass('default_message');

  function generateCensored(word) {
    var out = "";
    for (var i = 0;i<word.length;i++) {
        out += "*";
    }
    return out;
  }

  function censorString(inputString) {
    var wordArray = inputString.split(" ");
    var censor = false;
    var outputString = "";
    for (var i = 0; i < wordArray.length;i++) {
        censor = Math.round(Math.random());
        if (censor) {
            wordArray[i] = generateCensored(wordArray[i]);
        } else {
            wordArray[i] = wordArray[i].toUpperCase();
        }
    }
    for (var i = 0; i < wordArray.length; i++) {
        outputString += wordArray[i]+" ";
    }
    return outputString;
  }

  client.on('connect', function () {
    console.log("connected");
    client.subscribe('print');
    isConnected = true;
  });

  client.on('message', function (topic, message) {
    console.log(message.toString());
    client.end();
  });

  var constraints = {
    message: {
      presence: true,
      length: {
        maximum: 110
      }
    }
  };

  var form = document.querySelector("form#thermal-form");
  form.addEventListener("submit", function(ev) {
    ev.preventDefault();
    handleFormSubmit(form);
  });

  var inputs = document.querySelectorAll("textarea")

  var maxLength = parseInt($('#maxlength').html());

  $('#message').bind('input propertychange', function() {
    if ($('#message').val() != defaultMessage) {
      var messageLength = $('#message').val().length;
      if (messageLength > maxLength) {
        $('#message').val($('#message').val().substring(0,maxLength));
      }
      $('#maxlength').html(maxLength-messageLength);
    } else {
      $('#maxlength').html(maxLength);
    }
  });

  $('#message').bind('focusin', function() {
    $('#message').removeClass('default_message');
    $('#maxlength').html(maxLength);
  });

  $('#message').bind('focusout', function() {
    var message = $('#message').val();
    if(message.length == 0) {
      $('#message').val(defaultMessage);
      $('#message').addClass('default_message');
      $('#maxlength').html(maxLength);
    } else {
      $('#message').removeClass('default_message');
    }
  });

  function handleFormSubmit(form, input) {
    // validate the form against the constraints
    var errors = validate(form, constraints);
    if (!errors) {
      printMessage();
    }
  }

  function printMessage() {
    if ($('#message').val() != defaultMessage) {
      var message = censorString($('#message').val());
      message = iconv.encode(message, '437');
      var buffer = new Buffer(message);
      client.publish('print',buffer);
      $('#message').val("");
      $('#maxlength').html(maxLength);
      location.reload();
    }
  }
}());