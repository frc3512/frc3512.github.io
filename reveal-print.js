var link = document.createElement( 'link' );
link.rel = 'stylesheet';
link.type = 'text/css';
link.href = window.location.search.match( /print-pdf/gi ) ? '../../../reveal.js/css/print/pdf.scss' : '../../../reveal.js/css/print/paper.scss';
document.getElementsByTagName( 'head' )[0].appendChild( link );
