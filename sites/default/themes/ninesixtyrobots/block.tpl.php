<?php
// $Id: block.tpl.php,v 1.1.4.1 2009/08/05 18:55:54 add1sun Exp $
?>
<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="block block-<?php print $block->module ?>">
<?php if ($block->subject): ?>
  <h3 class="title"><?php print $block->subject ?></h3>
<?php endif;?>

  <div class="content">
    <?php print $block->content ?>
  </div>
</div>