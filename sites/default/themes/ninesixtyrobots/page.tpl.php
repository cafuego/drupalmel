<?php
// $Id: page.tpl.php,v 1.1.4.4 2009/08/05 18:55:54 add1sun Exp $
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <script type="text/javascript"><?php /* Needed to avoid Flash of Unstyled Content in IE */ ?> </script>
</head>
  <body>

    <!-- header starts-->
    <div id="header-wrap">
      <div id="header" class="container_16">
        <div id="header-main">
          <h1 id="logo-text"><a href="<?php print $front_page; ?>"><?php print $site_name; ?></a></h1>
          <p id="slogan"><?php print $site_slogan; ?></p>
        </div><!-- navigation -->
        <div id="nav">
          <?php print theme('links', $primary_links, array('class' => 'links primary-links')); ?>
        </div>
        <?php print $search_box; ?>
      </div>
    </div>
    <!-- header ends here -->
    
    <!-- content starts -->
    <div id="content-wrapper" class="container_16">

      <div id="breadcrumb" class="grid_16"><?php print $breadcrumb; ?></div>

      <!-- main -->
      <div id="main" class="<?php print ($left && $right) ? 'grid_8' : (($left || $right) ? 'grid_12' : 'grid_16') ?>">
        <?php if (!empty($title)): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
        <?php if (!empty($tabs)): ?><div class="tabs"><?php print $tabs; ?></div><?php endif; ?>
        <?php if (!empty($messages)): print $messages; endif; ?>
        <?php if (!empty($help)): print $help; endif; ?>
        <div id="content-output"> 
          <?php print $content; ?>
        </div><!-- /#content-output -->
      </div>
      <!-- main ends here -->

      <!-- sidebars starts here -->
      <?php if ($left || $right): ?>
      <div id="sidebars" class="<?php print ($left && $right) ? 'grid_8' : 'grid_4' ?>">

        <!-- left sidebar starts here -->
        <?php if ($left): ?>
        <div class="grid_4 alpha sidebar-left">
          <?php print $left; ?>
        </div>
        <?php endif; ?>
        <!-- left sidebar ends here -->

        <!-- right sidebar starts here -->
        <?php if ($right): ?>
        <div class="grid_4 omega sidebar-right">
          <?php print $right; ?>
        </div>
        <?php endif; ?>
        <!-- right sidebar ends here -->

      </div>
      <?php endif; ?>
      <!-- sidebars end here -->

    </div>
    <!-- content ends here -->

    <!-- footer starts here -->
    <div id="footer-wrapper" class="container_16">

      <!-- footer top starts here -->
      <div id="footer-content">

        <!-- footer left starts here -->
        <div class="grid_8" id="footer-left">
          <?php print $footer_left; ?>
        </div>
        <!-- footer left ends here -->

        <!-- footer right starts here -->
        <div class="grid_8" id="footer-right">
          <?php print $footer_right; ?>
        </div>
        <!-- footer right ends here -->

      </div>
      <!-- footer top ends here -->

      <!-- footer bottom starts here -->
      <div id="footer-bottom">
        <div id="footer-meta" class="clear-block">
          <?php if ($footer_message): ?>
          <p class="bottom-left"><?php print $footer_message; ?></p>
          <?php endif; ?>
  
          <?php if ($secondary_links): ?>
          <?php print theme('links', $secondary_links, array('class' => 'links secondary-links')); ?>
          <?php endif; ?>
        </div>

        <?php if ($footer): ?>
        <div id="footer-bottom-content">
          <?php print $footer; ?>
        </div>
        <?php endif; ?>
      </div>
      <!-- footer bottom ends here -->

    </div>
    <!-- footer ends here -->
    <?php print $closure; ?>
  </body>
</html>