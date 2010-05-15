// $Id: javascript.js,v 1.1.4.2 2009/08/12 23:29:56 add1sun Exp $

// Prefill the search box with Search... text.
$(document).ready(function(){
  $('#search input:text').autofill({
    value: "Search..."
  });
});