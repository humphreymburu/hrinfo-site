<?php

/**
 * @file
 * Template for the event node edit form.
 */
?>

<?php print drupal_render($form['title_field']); ?>
<?php print drupal_render($form['language']); ?>
<?php print drupal_render($form['field_event_category']); ?>
<?php print drupal_render($form['field_organizations']); ?>
<?php print drupal_render($form['field_event_date']); ?>
<?php print drupal_render($form['field_address']); ?>
<?php print drupal_render($form['body']); ?>
<?php print drupal_render($form['field_hid_contact_ref']); ?>
<?php print drupal_render($form['og_group_ref']); ?>
<?php print drupal_render($form['field_event_agenda']); ?>
<?php print drupal_render($form['field_coordination_hubs']); ?>
<?php print drupal_render($form['field_event_meeting_minutes']); ?>
<?php print drupal_render($form['field_disasters']); ?>
<fieldset class="hr-additional">
  <div class="toggle">
    <?php print drupal_render($form['field_location']); ?>
    <?php print drupal_render($form['field_themes']); ?>
    <?php print drupal_render($form['field_related_content']); ?>
    <?php print drupal_render($form['field_sectors']); ?>
    <?php print drupal_render($form['field_bundles']); ?>
  </div>
</fieldset>

<fieldset id="actions">
  <?php print drupal_render($form['actions']); ?>
</fieldset>

<?php print drupal_render_children($form); ?>
