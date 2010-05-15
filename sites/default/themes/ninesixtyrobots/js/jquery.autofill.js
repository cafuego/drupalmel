// $Id: jquery.autofill.js,v 1.1.4.1 2009/08/05 18:55:54 add1sun Exp $

// Auto-Fill Plugin
// Written by Joe Sak http://www.joesak.com/2008/11/19/a-jquery-function-to-auto-fill-input-fields-and-clear-them-on-click/
(function($){
  $.fn.autofill = function(options){
    var defaults = {
      value:'',
      defaultTextColor:"#ccc",
      activeTextColor:"#000",
      password: false
    };
    var options = $.extend(defaults,options);
    return this.each(function(){
      var obj=$(this);
      obj.css({color:options.defaultTextColor})
        .val(options.value)
        .focus(function(){
          if(obj.val()==options.value){
            obj.val("").css({color:options.activeTextColor});
            if (options.password && obj.attr('type') == 'text') {
              obj.attr('type', 'password');
            }
          }
        })
        .blur(function(){
          if(obj.val()==""){
            obj.css({color:options.defaultTextColor}).val(options.value);
            if (options.password && obj.attr('type') == 'password') {
              obj.attr('type', 'text');
            }
          }
        });
    });
  };
})(jQuery);