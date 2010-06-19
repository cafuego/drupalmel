<div id="wysiwyg-toolbar-designer">
  <h4><?php print t('Available buttons'); ?></h4>
  <div id="toolbar-available-buttons">
    <?php foreach ($form['buttons']['#value'] as $button): ?>
    <span class="wysiwyg-button wysiwyg-button-<?php print $button['plugin'] . '-' . $button['button'];?>">
      <?php echo $button['title']; ?>
    </span>
    <?php endforeach;?>
  </div>

  <h4><?php print t('Toolbar'); ?></h4>

  <div class="warning">
    <span class="warning">*</span> <?php print t('Changes will not be saved until the form is submitted.'); ?>
  </div>

  <div id="toolbar-rows"></div>

  <div id="toolbar-actions">
    <a title="<?php print t('Add new row') ;?>" class="add-toolbar-row" href="javascript:;">&nbsp;</a>
    <input id="reset-design" type="button" value="<?php print t('Reset'); ?>"/>
  </div>

  <div class="toolbar-row-template">
    <a href="javascript:;" class="row-handler handler">&nbsp;</a>
    <a href="javascript:;" class="add-group">&nbsp;</a>
  </div>

  <div class="toolbar-group-template">
    <a href="javascript:;" class="group-handler handler">&nbsp;</a>
  </div>
  <?php echo drupal_render($form['toolbar']) ?>
</div>
