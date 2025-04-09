/**
 * Splits a data frame into an array of elements using a comma separator.
 *
 * Use this function to break a string (like "value1,value2,value3") into
 * individual pieces, which can then be displayed or processed in your project.
 *
 * @param[in]  frame   A string containing the data frame.
 *                     Example: "value1,value2,value3"
 *
 * @return     An array of strings with the split elements.
 *             Example: ["value1", "value2", "value3"]
 *
 * @note You can declare global variables outside this function if needed
 *       for storing settings or keeping state between calls.
 */
 
var lastFrame = null;
 
function parse(frame) {
    var stuff = frame.split(',');
    for(var i = 0; i < frame.length; i++) {
        if(stuff[i] == "") {
            console.log("Keeping element " + i + ": " + lastFrame[i]);
            stuff[i] = lastFrame[i];          
        }
    }
    lastFrame = stuff;
    console.log("received " + JSON.stringify(stuff));
    return stuff;
}

