'use strict';
(function() {

  var loadFile = function(url, callback) {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', url, true);
    xhr.responseType = 'arraybuffer';
    xhr.send();
    xhr.onload = function() {
      if (xhr.status !== 200) {
        console.log('Unexpected status code ' + xhr.status + ' for ' + url);
        return false;
      }
      callback(new Uint8Array(xhr.response));
    };
  }

  var main = function() {
    var video = document.querySelector('video');

    var objectUrls = [];

    loadFile(
        'test1.mp4',
        function(uInt8Array) {
          var blob = new Blob([uInt8Array], {type: 'video/mp4'});
          objectUrls.push(URL.createObjectURL(blob));

          loadFile('test2.mp4',
              function(uInt8Array) {
                var blob = new Blob([uInt8Array], {type: 'video/mp4'});
                objectUrls.push(URL.createObjectURL(blob));

                video.src = objectUrls[objectUrls.length - 1];
                window.setTimeout(
                  function() {
                    video.src = objectUrls[0];
                    window.setTimeout(
                      function() {
                        video.src = objectUrls[objectUrls.length - 1];
                      },
                      5000);
                  },
                  5000);
              });
        });

    window.timeout
  };

  main();

})();
