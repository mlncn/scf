// $Id: live.comment.js,v 1.1 2008/06/28 18:41:15 Gurpartap Exp $

Drupal.behaviors.liveCommentPreview = function() {
  textarea = $("textarea#edit-comment");
  comment_form = $("form#comment-form");

  textarea.each(function() {
    var element = Drupal.settings.live.comment.element;
    var box = '<div id="live-preview-container"><div id="live-comment-preview"></div><div id="live-comment-preview-background"></div></div>';

    $(this).one('click', function() {
      switch (Drupal.settings.live.comment.placement) {
        case 'prepend':
          $(box).prependTo(element);
          break;
        case 'append':
          $(box).appendTo(element);
          break;
        case 'replace':
          $(element).html(box);
          break;
        default:
          $(box).appendTo(comment_form);
          break;
      }
    }).bind('keyup mouseup blur', updateCommentPreview).each(function() {
      var submit_button = comment_form.find('input#edit-submit');
      if (submit_button.size()) {
        // Hide "Preview comment" button, iff "Post comment" button is present.
        submit_button.siblings("input#edit-preview").hide();
      }
    });

    timer = undefined;

    function updateCommentPreview() {
      if (timer) {
        clearTimeout(timer);
        timer = undefined;
      }

      timer = setTimeout(function() {
        var comment_div = $("div#live-comment-preview");
        var comment_div_background = $('div#live-comment-preview-background');
        var progress_panel = $('<div class="live-progress-panel">Loading...</div>');
        var body = textarea.val();
    
        if (body == "") {
          comment_div.html("").parent().slideUp();
          return;
        }

        var name = $("input#edit-name").val() || "";
        var homepage = $("input#edit-homepage").val() || "";
        var subject = $("input#edit-subject").val();
        var format = $("input[@name=format][@type=radio][@checked]").val() || 1;
        var token = $("input[@name=form_token]#edit-form-token").val() || "";

        var base_url = Drupal.settings.basePath;
        var module_path = Drupal.settings.live.comment.module_path + '/';
        var token_value = Drupal.settings.live.comment.token_value;

        $.ajax({
          type: "POST",
          url: base_url + "index.php?q=live/comment/preview",
          data: {
            name: name,
            homepage: homepage,
            subject: subject,
            body: body,
            format: format,
            token: token,
            token_value: token_value
          },
          timeout: 5000,
          dataType: 'json',
          beforeSend: function() {
            progress_panel.appendTo('body').hide().fadeIn(200);
          },
          success: function(data){
            if (typeof data['html'] == 'undefined' || data['html'] != 0) {
              $('div#live-preview-container').slideDown().fadeTransition({
                html: data['html'],
                first: comment_div,
                second: comment_div_background
              });
            }
            setTimeout(function() {
              progress_panel.fadeTo(500, 0, function() {
                $(this).remove();
              });
            }, 500);
          },
          error: function() {
            progress_panel.html('Error requesting data!');
            comment_div.parent().slideUp();
            setTimeout(function() {
              progress_panel.fadeTo(1500, 0, function() {
                $(this).remove();
              });
            }, 2500);
          }
        });
      }, Drupal.settings.live.comment.refresh_delay);
    }
  });
};

