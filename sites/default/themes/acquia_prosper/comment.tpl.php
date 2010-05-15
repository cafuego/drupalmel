<?php
// $Id: comment.tpl.php,v 1.1 2009/08/19 04:28:52 sociotech Exp $
?>

<div class="comment-wrapper">
  <div class="comment <?php print $comment_classes;?> clear-block <?php if ($picture): ?>withpicture<?php endif; ?>">

    <?php if ($picture): ?>
    <div class="withpicture-column-left">
      <?php print $picture ?>
    </div>
    <?php endif; ?>
    
    <?php if ($picture): ?>
    <div class="withpicture-column-right">
    <?php endif; ?>
      <?php if ($comment->new): ?>
      <a id="new"></a>
      <span class="new"><?php print $new ?></span>
      <?php endif; ?>
      <div class="submitted">
        <?php print $submitted ?>
      </div>
      
      <div class="content">
        <div class="comment-top-left"></div><!-- /comment-top-left -->
        <div class="content-inner">
          <h3 class="title"><?php print $title ?></h3>
          <?php print $content ?>
          <?php if ($signature): ?>
          <div class="signature">
            <?php print $signature ?>
          </div>
          <?php endif; ?>
          <?php if ($links): ?>
          <div class="links">
            <?php print $links ?>
          </div>
          <?php endif; ?>
        </div><!-- /content-inner -->
      </div><!-- /content -->

    <?php if ($picture): ?>
    </div><!-- /withpicture-column-right -->
    <?php endif; ?>
  </div><!-- /comment -->
</div><!-- /comment-wrapper -->