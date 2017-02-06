/**
 * Generates a table of contents for your document based on the headings
 * present. Anchors are injected into the document and the entries in the table
 * of contents are linked to them. The table of contents will be generated
 * inside of the first element with the id `toc`.
 *
 * @param {HTMLDOMDocument} documentRef Optional A reference to the document
 *                                      object. Defaults to `document`.
 */
function tableOfContents(documentRef) {
    var documentRef = documentRef || document;
    var toc = documentRef.getElementById('toc');
    var headings = [].slice.call(documentRef.body.querySelectorAll('h2'));
    headings.forEach(function (heading, index) {
        // Create link for table of contents
        var link = documentRef.createElement('a');
        link.setAttribute('href', '#' + heading.getAttribute('id'));
        link.textContent = heading.textContent;

        // Append link to div
        var div = documentRef.createElement('div');
        div.setAttribute('class', heading.tagName.toLowerCase());
        div.appendChild(link);

        // Append div to table of contents
        toc.appendChild(div);
    });
}

try {
     module.exports = htmlTableOfContents;
} catch (e) {
    // module.exports is not defined
}
