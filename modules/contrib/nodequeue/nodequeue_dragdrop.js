$(document).ready(function() {
  $('a.nodequeue-remove').css("display", "block");
  $('a.nodequeue-remove').click(function() {
    a = $(this).attr('id');
    a = "#" + a.replace('nodequeue-remove-', 'edit-') + '-position';
    $(a).val('r');
    //Hide the current row
    $(this).parent().parent().addClass('hidden').hide();
    //Restripe the table
    // :even and :odd are reversed because jquery counts from 0 and
    // we count from 1, so we're out of sync.
    $('table.nodequeue-dragdrop tr:not(:hidden)')
     .filter(':odd').filter('.odd')
      .removeClass('odd').addClass('even')
    .end().end()
    .filter(':even').filter('.even')
      .removeClass('even').addClass('odd');
    return false;
  });
});