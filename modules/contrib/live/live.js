// $Id: live.js,v 1.1 2008/06/28 18:41:15 Gurpartap Exp $

(function($) {
  var runonce = true;
  var saved_html = '';
  var old_html = '';
  $.fn.fadeTransition = function(options) {
      return this.each(function() {   
          $.fadeTransition(this, options);
      });
  };

  $.fadeTransition = function(container, options) {
    var settings = {
        'html':    '',
        'speed':   'normal',
        'first':   null,
        'second':  null
    };
    if (options) {
      $.extend(settings, options);
    }
    old_html = saved_html;
    saved_html = settings.html;

    var first = settings.first;
    var second = settings.second;
    var temp;

    if ((typeof first == 'undefined') || (typeof second == 'undefined')) {
      return true;
    }

    if (runonce) {
      $(container).css({'position': 'relative'});
      first.css({'z-index': '2', 'position': 'absolute', 'width': '100%'}).hide();
      second.css({'z-index': '1', 'position': 'absolute', 'width': '100%'}).hide();
      runonce = false;
    };

    setTimeout(function() {
      first.html(settings.html).fadeIn(settings.speed, function() {
				removeFilter($(this)[0]);
			});
      second.fadeOut(settings.speed, function() {
        $(this).html(settings.html);
      });

      if (settings.html.length >= old_html.length) {
        var containerHeight = first[0].offsetHeight > second[0].offsetHeight ? first[0].offsetHeight : second[0].offsetHeight;
        var containerWidth = first[0].offsetWidth > second[0].offsetWidth ? first[0].offsetWidth : second[0].offsetWidth;
      }
      else {
        var containerHeight = first[0].offsetHeight < second[0].offsetHeight ? first[0].offsetHeight : second[0].offsetHeight;
        var containerWidth = first[0].offsetWidth < second[0].offsetWidth ? first[0].offsetWidth : second[0].offsetWidth;
      }
      $(container).animate({'height': containerHeight, 'width': containerWidth});
    }, 100);

    $(first).show();

    // Interchange first and seconds.
    temp = first;
    first = second;
    second = temp;
  };

})(jQuery);

/**
 * Remove Opacity-Filter in IE.
 */
function removeFilter(element) {
	if (element.style.removeAttribute){
		element.style.removeAttribute('filter');
	}
}

