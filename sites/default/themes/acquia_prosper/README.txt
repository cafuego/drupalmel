-- SUMMARY --

Acquia Prosper is an advanced Drupal theme, with a monochromatic look and clean lines.  It is designed as an Ubercart e-commerce theme but is extremely flexible for any type of site. 

This theme is a subtheme of the Fusion Core base theme.  Acquia Prosper + Fusion is a little different from most Drupal themes you may be used to.  Positioning and block styles are controlled through Drupal's UI via the Skinr module, which is required if you want to actually use styles.  These configurable options give Acquia Prosper an unparalleled amount of flexibility and features.


-- REQUIREMENTS --

Fusion Core:
  http://drupal.org/project/fusion

Skinr:
  http://drupal.org/project/skinr


-- INSTALLATION --

* Download both Acquia Prosper and Fusion (http://drupal.org/project/fusion)

* Unpack them as usual within your themes folder.  You'll see two themes in the Fusion folder -- Fusion Core (the base theme), and Fusion Starter (a commented starter theme for creating your own subthemes)

* Enable Fusion Core and Acquia Prosper on your themes page on example.com/admin/build/themes

* Set Acquia Prosper as your default theme

* Download and enable the Skinr module

   - The dev snapshot is currently recommended for the most up-to-date version: http://ftp.drupal.org/files/projects/skinr-6.x-1.x-dev.tar.gz

* You may need to give your administrator role Skinr permissions on example.com/admin/user/permissions


-- CONFIGURATION --

* You can control many settings on Acquia Prosper's theme settings page: example.com/admin/build/themes/settings/acquia_prosper
   - Settings include menus, fixed vs fluid page width, sidebar widths, typography, search results, and administrator/developer helper features

* Superfish dropdown menus can be enabled by selecting "Expanded" next to the parent menu item in your menu configuration

* Go to your Blocks page at example.com/admin/build/block and place blocks into regions as desired, save your settings

* When hovering your cursor over blocks, a gear icon will appear -- you can use this to get to the block configuration page for that specific block

* On each block's configuration page, a "Skinr" section will appear, where you can set block styles

  NOTE: Currently Skinr's user interface is in a beta state, and only supports checkboxes at this time. Improvements are coming soon, but in the meantime, you will need to exercise some self control and not enable multiple conflicting styles at the same time. For example, only enable a single width option and a single alignment option at once. Many of the styles may also not layer well.

* Use these width and positioning options for blocks to configure your layout

* Use the style options to set up different visual styles for different blocks


-- UBERCART FEATURES --

If you are using the Ubercart e-commerce suite, this theme has lots of enhanced features for your site!

* Custom product template and styling via a node-product.tpl.php file, which themes the product images, price, attributes, product info, and add to cart form

* A "simple comments" style which is ideal for user product reviews using the Fivestar module

* Custom icon for your shopping cart

* Styles for shopping cart block on light or dark backgrounds

* Custom styling for product subcategory links in catalog

* Themed catalog grid view

* Emphasized theming of key customer action buttons, such as Checkout and Review Order, and de-emphasis of "cancel" links

* Custom theming of cart and checkout pages


-- EXAMPLES --

The following are recommended block placements/styles and other settings for Acquia Prosper to get you started exploring what Fusion can do!

* Set the "Acquia Prosper: Gray rounded title background, list styling" style for menu blocks in your sidebar

* Add the Ubercart shopping cart block to the Header region, give it a width of 4 units, set the block to float to the right, and give it the "Acquia Prosper: Shopping cart - Dark background" block style

* Try adding a menu such as your primary links block to the Footer region, and set it to use either the "Single line inline menu with separators" style for a one level menu, or "Multi-column menu with headers" style for a nested menu

* Edit a content type with commenting enabled, go to the Skinr section, and select "Acquia Prosper: Comments - edgy" for the comment style for special comment theming

* Add the user login block to the Header Top region, and set it to the "Horizontal user login" style for a sleek login form

* Add some promotional text to a block in the Preface Bottom region, and give it the "Acquia Prosper: Gradient background with padding and thick border" for a prominent callout style

* Use the Quicktabs module (http://drupal.org/project/quicktabs) to create a jQuery tabbed block, and set that block's style to "Acquia Prosper: Tabbed block" for custom matching tabs

* You can also set many of these styles on Views, content types, or Panels!


-- CREDITS --

This theme has been made possible by:

* TOP NOTCH THEMES
  Concept, theming, and support by:
  http://www.topnotchthemes.com

* ACQUIA
  Sponsored by:
  http://acquia.com

* ARKIKSTUDIO
  Graphic design by:
  http://www.arkikstudio.com/

* UBERCART
  Inspiration and input from:
  http://www.ubercart.org/
