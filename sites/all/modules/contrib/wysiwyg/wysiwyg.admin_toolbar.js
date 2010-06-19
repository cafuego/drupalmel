// $Id$

Drupal.behaviors.wysiwygToolbarDesigner = function(context) {
  var settings = Drupal.settings.wysiwyg_toolbar;
  var workspace = $('#wysiwyg-toolbar-designer');
  var designArea = $('#toolbar-rows');
  var changeNotification = $('#wysiwyg-toolbar-designer div.warning');
  var availableButtons = $('#toolbar-available-buttons');
  var separator = $('.wysiwyg-button-default-Separator',availableButtons);

  var createRow = function(noGroup) {
    var row = $('.toolbar-row-template',workspace).clone().removeClass('toolbar-row-template');
    row.addClass('toolbar-row').sortable({
      handle: '.group-handler',
      revert: true,
      items: '.toolbar-group',
      addClasses: false,
      connectWith: '#toolbar-rows .toolbar-row',
      stop: function(event,ui) {
        changeNotification.fadeIn();
      }
    });

    if (settings['toolbar groups']) {
      row.find('.add-group').click(function() {
        var group = createGroup();
        row.append(group);
        row.sortable('refresh');
      });
    }
    else {
      row.find('.add-group').hide();
      row.addClass('single-group');
    }

    // add required group
    if (!noGroup) {
      var group = createGroup();
      row.append(group);
    }

    return row;
  };

  var createGroup = function() {
    var group = $('.toolbar-group-template').clone().removeClass('toolbar-group-template');
    group.addClass('toolbar-group');

    group.sortable({
      revert: true,
      items: '.wysiwyg-button',
      connectWith: '#toolbar-rows .toolbar-group',
      addClasses: false,
      stop: function(event,ui) {
        changeNotification.fadeIn();
      }
    });

    group.droppable({
      accept: '.template-button',
      drop: function(event,ui) {
        var button = ui.draggable.clone();
        button.removeClass('template-button').addClass('toolbar-button');

        // Disable this button in template area.
        ui.draggable.hide();
        separator.show();

        $(this).append(button).sortable('refresh');
        changeNotification.fadeIn();
      }
    });

    return group;
  }

  var reset = function() {
    $('.toolbar-row',designArea).remove();

    // Enable all buttons and then disable it later.
    $('.wysiwyg-button',availableButtons).show();


    for (var i in settings.toolbar) {
      var groups = settings.toolbar[i];
      var row = createRow(true);

      for (var j in groups) {
        var group = createGroup();
        var buttons = groups[j];

        for (var k in buttons) {
          var buttonClass = '.wysiwyg-button-' + buttons[k].plugin + '-' + buttons[k].button;
          var template_button = $(buttonClass,$('#toolbar-available-buttons'));

          if (template_button.length) {

            button = template_button.clone().show();
            button.removeClass('template-button').addClass('toolbar-button');

            group.append(button);

            // Disable button in template area.
            template_button.hide();
          }
        }

        row.append(group);
      }

      designArea.append(row);
    }

    // Make sure we always have at least one row.
    if (!settings['toolbar rows']) {
      if ($('.toolbar-row',designArea).length <= 0) {
        var row = createRow();
        designArea.append(row);
      }
    }

    separator.show();
    changeNotification.fadeOut();
  }

  $('.add-toolbar-row',workspace).click(function(){
    // clone from toolbar template
    var row = createRow();

    // Append row to design area.
    designArea.append(row).sortable('refresh');
    changeNotification.fadeIn();
  });

  $('.wysiwyg-button',availableButtons).addClass('template-button').draggable({
    handle: '.handler',
    helper: 'clone',
    revert: 'invalid',
    addClasses: false
  });

  availableButtons.droppable({
    accept: '.toolbar-button, .toolbar-group, .toolbar-row',
    drop: function(event, ui) {
      var item = ui.draggable;
      var parent = item.parent();

      // Guarantee there is at least 1 row and 1 group.
      if (item.hasClass('toolbar-row') && parent.find('.toolbar-row').not('.ui-sortable-placeholder').length == 1) {
        return;
      } else if (item.hasClass('toolbar-group') && parent.find('.toolbar-group').not('.ui-sortable-placeholder').length == 1) {
        return;
      }

      var buttons = $('.toolbar-button', item);
      if (item.hasClass('toolbar-button')) {
        buttons.add(item);
      }
      
      // Remove each button and enable in template.
      buttons.each(function(){
        var button_id = /wysiwyg-button-([^-]+-[^\s]+)/.exec($(this).attr('class'));
        $('.wysiwyg-button-' + button_id[1]).show();
      });
      
      item.parent().sortable('refresh');
      item.remove();

      changeNotification.fadeIn();
    }
  });

  $('#toolbar-rows').sortable({
    items: '.toolbar-row',
    handle: '.row-handler',
    addClass: false,
    stop: function(event, ui) {
      changeNotification.fadeIn();
    }
  });

  /* Design actions buttons. */
  $('#reset-design').click(function() {
    if (!changeNotification.is(':hidden') && confirm(Drupal.t('Do you want to reset the changes ?')))
      reset();
  });


  $('#wysiwyg-profile-form').submit(function() {
    // Prepare toolbar data to submit.
    var toolbar = "";

    designArea.find('.toolbar-row').each(function(key,rowDom){
      var row = "";

      $('.toolbar-group',rowDom).each(function(key,groupDom){
        var group = "";

        $('.wysiwyg-button',groupDom).each(function(key,button){
          var cls = /wysiwyg-button-([^-]+)-([^\s]+)/.exec($(button).attr('class'));
          group += cls[1] + "." + cls[2] + ",";
        })

        row += group + "|";
      });

      toolbar += row + "\n";
    });
    
    // Assign to hidden field.
    $('#edit-toolbar').val(toolbar);
  });

  reset();

  if (!settings['toolbar rows']) {
    $('.add-toolbar-row').hide();
  }
};
