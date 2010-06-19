// $Id: fckeditor.config.js,v 1.6 2009/06/08 22:20:38 sun Exp $

Drupal = window.parent.Drupal;

/**
 * Fetch and provide original editor settings as local variable.
 *
 * FCKeditor does not support to pass complex variable types to the editor.
 * Instance settings passed to FCKinstance.Config are temporarily stored in
 * FCKConfig.PageConfig.
 */
var wysiwygFormat = FCKConfig.PageConfig.wysiwygFormat;
var wysiwygSettings = Drupal.settings.wysiwyg.configs.fckeditor[wysiwygFormat];
var pluginSettings = Drupal.settings.wysiwyg.plugins[wysiwygFormat];

/**
 * Apply format-specific settings.
 */
for (var setting in wysiwygSettings) {
  FCKConfig[setting] = wysiwygSettings[setting];
}

/**
 * Initialize this editor instance.
 */
Drupal.wysiwyg.editor.instance.fckeditor.init(window);

/**
 * Register native plugins for this input format.
 *
 * Parameters to Plugins.Add are:
 * - Plugin name.
 * - Languages the plugin is available in.
 * - Location of the plugin folder; <plugin_name>/fckplugin.js is appended.
 */
for (var plugin in pluginSettings['native']) {
  // Languages and path may be undefined for internal plugins.
  FCKConfig.Plugins.Add(plugin, pluginSettings['native'][plugin].languages, pluginSettings['native'][plugin].path);
}

/**
 * Register Drupal plugins for this input format.
 *
 * Parameters to addPlugin() are:
 * - Plugin name.
 * - Format specific plugin settings.
 * - General plugin settings.
 * - A reference to this window so the plugin setup can access FCKConfig.
 */
for (var plugin in pluginSettings.drupal) {
  Drupal.wysiwyg.editor.instance.fckeditor.addPlugin(plugin, pluginSettings.drupal[plugin], Drupal.settings.wysiwyg.plugins.drupal[plugin], window);
}

