function loadFile(url, data, callback, errorCallback, userParam) {
    // Set up an asynchronous request
    var request = new XMLHttpRequest();
    request.open('GET', url, true);
    request.responseType = "text";

    // Hook the event that gets called as the request progresses
    request.onreadystatechange = function () {
        // If the request is "DONE" (completed or failed)
        if (request.readyState == 4) {
            // If we got HTTP status 200 (OK)
            if (request.status == 200) {
                callback(request.responseText, data, userParam)
            } else { // Failed
                errorCallback(url, userParam);
            }
        }
    };

    try{
      request.send(null);
    } catch(err) {
    }
}

function loadFiles(urls, callback, errorCallback, userParam) {
    var numUrls = urls.length;
    var numComplete = 0;
    var result = [];

    // Callback for a single file
    function partialCallback(text, urlIndex, userParam) {
        result[urlIndex] = text;
        numComplete++;

        // When all files have downloaded
        if (numComplete == numUrls) {
            callback(result, userParam);
        }
    }

    for (var i = 0; i < numUrls; i++) {
        loadFile(urls[i], i, partialCallback, errorCallback, userParam);
    }
}

function getShaderFromSource(gl, shaderStr, isFragment) {
    var shader;
    if (isFragment == true) {
        shader = gl.createShader(gl.FRAGMENT_SHADER);
    } else {
        shader = gl.createShader(gl.VERTEX_SHADER);
    }

    gl.shaderSource(shader, shaderStr);
    gl.compileShader(shader);

    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        alert(gl.getShaderInfoLog(shader));
        gl.deleteShader(shader);
        return null;
    }

    return shader;
}

function linkProgram(gl, fragmentShader, vertexShader) {
    // Create the program object
    var programObject = gl.createProgram ( );

    if ( programObject == 0 )
        return 0;

    gl.attachShader ( programObject, vertexShader );
    gl.attachShader ( programObject, fragmentShader );

    // Link the program
    gl.linkProgram ( programObject );

    // Check the link status
    linked = gl.getProgramParameter ( programObject, gl.LINK_STATUS );

    if ( !linked )
    {
        alert( gl.getProgramInfoLog ( programObject ) );
        gl.deleteProgram ( programObject );
        return 0;
    }
    // Free up no longer needed shader resources
    gl.deleteShader ( vertexShader );
    gl.deleteShader ( fragmentShader );

    return programObject;
}

function getShader(gl, id) {
    var shaderScript = document.getElementById(id);
    if (!shaderScript) {
        return null;
    }

    var str = "";
    var k = shaderScript.firstChild;
    while (k) {
        if (k.nodeType == 3)
            str += k.textContent;
        k = k.nextSibling;
    }

    var shader;
    if (shaderScript.type == "x-shader/x-fragment") {
        shader = gl.createShader(gl.FRAGMENT_SHADER);
    } else if (shaderScript.type == "x-shader/x-vertex") {
        shader = gl.createShader(gl.VERTEX_SHADER);
    } else {
        return null;
    }

    gl.shaderSource(shader, str);
    gl.compileShader(shader);

    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        alert(gl.getShaderInfoLog(shader));
        gl.deleteShader(shader);
        return null;
    }

    return shader;
}

function loadProgram(gl, vertShaderId, fragShaderId)
{
   var vertexShader;
   var fragmentShader;
   var programObject;
   var linked;

   // Load the vertex/fragment shaders
   vertexShader = getShader(gl, vertShaderId);
   if ( vertexShader == 0 )
      return 0;

   fragmentShader = getShader(gl, fragShaderId);
   if ( fragmentShader == 0 )
   {
      gl.deleteShader( vertexShader );
      return 0;
   }

   // Create the program object
   programObject = gl.createProgram ( );

   if ( programObject == 0 )
      return 0;

   gl.attachShader ( programObject, vertexShader );
   gl.attachShader ( programObject, fragmentShader );

   // Link the program
   gl.linkProgram ( programObject );

   // Check the link status
   linked = gl.getProgramParameter ( programObject, gl.LINK_STATUS );

   if ( !linked )
   {
       alert( gl.getProgramInfoLog ( programObject ) );
       gl.deleteProgram ( programObject );
       return 0;
   }

   // Free up no longer needed shader resources
   gl.deleteShader ( vertexShader );
   gl.deleteShader ( fragmentShader );

   return programObject;
}

