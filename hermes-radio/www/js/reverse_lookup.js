var geocoder;
var gWatchID;
var gOptions = { frequency: 1000 }; 
var latitude;
var longtitude;
var accuracy;
var data = [["latitude","longitude"]];
var options =
    {
        enableHighAccuracy: true,
        timeout: 10000,
        maximumAge: 0
    };

$(document).ready(function () {
    geocoder = new google.maps.Geocoder();
});
window.navigator = window.navigator || {};
var alert = function () {
    $('#lblLatitudeA').html("Getting Location...");
    $('#lblLongitudeA').html("");
    $('#lblAccuracyA').html("");
    $('#lblLocation').html("");


    var success = function (position) {
        latitude = parseFloat(position.coords.latitude);
        longitude = parseFloat(position.coords.longitude); 
        accuracy = parseFloat(position.coords.accuracy);
        data.push([latitude,longitude]);

        $('#lblLatitudeA').html("Latitude: " + latitude);
        $('#lblLongitudeA').html("Longitude: " + longitude);
        $('#lblAccuracyA').html("Accuracy: " + accuracy + " metres");
           
    };

    var failure = function (error) {
        alert("Error: " + error.message);
        $('#lblLatitudeA').html("");
        $('#lblLongitudeA').html("");
        $('#lblAccuracyA').html("");
        $('#lblLocationA').html("");
    };

    if (navigator.geolocation) {
        navigator.geolocation.getCurrentPosition(success, failure, options);
    } else {
        alert("Geolocation is not supported");
    }
};
var getCurrentPosition = function () {

    $('#lblLatitude').html("Getting Location...");
    $('#lblLongitude').html("");
    $('#lblAccuracy').html("");

    var success = function (position) {
        latitude = parseFloat(position.coords.latitude);
        longitude = parseFloat(position.coords.longitude); 
        accuracy = parseFloat(position.coords.accuracy);
        $('#lblLatitude').html("Latitude: " + latitude);
        $('#lblLongitude').html("Longitude: " + longitude);
        $('#lblAccuracy').html("Accuracy: " + accuracy + " metres");
        
        for (i = 1; i < data.length; i++) { 
            if (latitude.toFixed(4)==data[i][0].toFixed(4) && longitude.toFixed(4) ==data[i][1].toFixed(4)){
                navigator.vibrate([1000, 500, 1000, 500, 1000]);
            }
            else{
                navigator.vibrate(0);
            }
        };

    };

    var failure = function (error) {
        alert("Error: " + error.message);
        $('#lblLatitude').html("");
        $('#lblLongitude').html("");
        $('#lblAccuracy').html("");
    };

    if (navigator.geolocation) {
        navigator.geolocation.getCurrentPosition(success, failure, options);
        gWatchID = navigator.geolocation.watchPosition(success,failure, gOptions);
    } else {
        alert("Geolocation is not supported");
    }
};
var safe = function () {

    
    var success = function (position) {
        latitude = parseFloat(position.coords.latitude);
        longitude = parseFloat(position.coords.longitude); 
        accuracy = parseFloat(position.coords.accuracy);
        
        for (i = 1; i < data.length; i++) { 
            if (latitude.toFixed(4)==data[i][0].toFixed(4) && longitude.toFixed(4) ==data[i][1].toFixed(4)){
                data.splice(i,1);
            };
        };
    };

    var failure = function (error) {
        alert("Error");
    };

    if (navigator.geolocation) {
        navigator.geolocation.getCurrentPosition(success, failure, options);
    } else {
        alert("Geolocation is not supported");
    }
    navigator.vibrate([1000, 500, 1000, 500, 2000]);
};
    

