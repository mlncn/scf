// $Id: live.node.js,v 1.1 2008/06/28 18:41:15 Gurpartap Exp $

Drupal.behaviors.liveNodePreview = function() {
  var element = Drupal.settings.live.node.element;
  var node_type = Drupal.settings.live.node.node_type;
  var base_url = Drupal.settings.basePath;
  var module_path = Drupal.settings.live.node.module_path + '/';
  var token_value = Drupal.settings.live.node.token_value;

  $('#node-form input#edit-preview').one('click', function() {
    var box = '<div id="live-preview-container"><div id="live-node-preview"></div><div id="live-node-preview-background"></div></div>';
    switch (Drupal.settings.live.node.placement) {
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
        $(box).appendTo('form#node-form');
        break;
    }
  }).bind('click', updateNodePreview);

  function updateNodePreview() {
    var preview_button = $(this);
    var title = $('input#edit-title').val() || '';
    var teaser_include = $('input#edit-teaser-include').attr('checked') ? 1 : 0;
    var teaser_js = $('textarea#edit-teaser-js').val() || '';
    var body = $('textarea#edit-body').val() || '';
    var username = $('input#edit-name').val() || '';
    var date = $('input#edit-date').val() || '';
    var format = $('input[@name=format][@type=radio][@checked]').val() || 1;
    var token = $('input[@name=form_token]#edit-'+ node_type +'-node-form-form-token').val() || "";

    var node_div = $('div#live-node-preview');
    var node_div_background = $('div#live-node-preview-background');
    var progress_panel = $('<div class="live-progress-panel">Loading...</div>');

    $.ajax({
      type: "POST",
      url: base_url + 'index.php?q=live/node/preview',
      data: {
        node_type: node_type,
        title: title,
        teaser_include: teaser_include,
        teaser_js: teaser_js,
        body: body,
        username: username,
        date: date,
        format: format,
        token: token,
        token_value: token_value
      },
      dataType: 'json',
      timeout: 5000,
      beforeSend: function() {
        progress_panel.appendTo('body').hide().fadeIn(200);
      },
      success: function(data){
        if (typeof data['html'] == 'undefined' || data['html'] != 0) {
          $('div#live-preview-container').slideDown().fadeTransition({
            html: data['html'],
            first: node_div,
            second: node_div_background
          });
          preview_button.val(Drupal.t('Update Preview'));
        }
        else if (typeof data['error'] == 'undefined' || data['error'] != 0) {
          $('div#live-preview-container').fadeTransition({
            html: data['error'],
            first: node_div,
            second: node_div_background
          });
          preview_button.val(Drupal.t('Preview'));
        }
        setTimeout(function() {
          progress_panel.fadeTo(500, 0, function() {
            $(this).remove();
          });
        }, 500);
      },
      error: function() {
        progress_panel.html('Error requesting data!');
        preview_button.val(Drupal.t('Preview'));

        node_div.parent().slideUp();

        setTimeout(function() {
          progress_panel.fadeTo(1500, 0, function() {
            $(this).remove();
          });
        }, 2500);
      }
    });
    return false;
  }
}

