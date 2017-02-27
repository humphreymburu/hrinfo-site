<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 * @see html.tpl.php
 */
?>
<div id="root">
  <header class="cd-hri-header hidden-print" role="banner">
    <div class="cd-hri-global-header">
      <div class="cd-hri-container cd-hri-global-header__inner container">
        <div class="cd-hri-global-header__sites cd-hri-dropdown">
          <button type="button" class="cd-hri-global-header__sites-btn" data-toggle="dropdown" id="cd-hri-related-platforms-toggle">
            <?php print t('Related platforms'); ?>
            <i class="fa fa-caret-down" aria-hidden="true"></i>
          </button>
          <ul class="cd-hri-dropdown-menu dropdown-menu" role="menu" id="cd-hri-related-platforms" aria-hidden="true" aria-labelledby="cd-hri-related-platforms-toggle">
            <li><a href="https://fts.unocha.org/">Financial Tracking Services</a></li>
            <li><a href="https://humdata.org/">Humanitarian Data Exchange</a></li>
            <li><a href="https://humanitarian.id/">Humanitarian ID</a></li>
            <li><a href="https://reliefweb.int/">ReliefWeb</a></li>
          </ul>
        </div>
        <div class="cd-hri-global-header__nav">
          <div class="cd-hri-nav cd-hri-nav--secondary">
            <?php print $hr_favorite_spaces; ?>
            <?php print render($page['top']); ?>
          </div>

        </div>
      </div>
    </div>
    <div class="cd-hri-site-header">
      <div class="cd-hri-container cd-hri-site-header__inner container">
        <a href="<?php print $front_page; ?>" class="cd-hri-site-header__logo">
          <span class="sr-only">Humanitarian Response</span>
        </a>
        <div class="cd-hri-site-header__search">
          <?php print render($page['branding']); ?>
        </div>
        <div class="cd-hri-dropdown">
          <button type="button" id="cd-hri-nav-toggle" class="cd-hri-site-header__nav-toggle" data-toggle="dropdown">
            <span class="cd-hri-site-header__nav-toggle-inner" aria-hidden="true">
            </span>
            <span class="sr-only"><?php print t('Main menu') ?></span>
          </button>
          <nav role="navigation" class="cd-hri-site-header__nav dropdown-menu" aria-labelledby="cd-hri-nav-toggle">
            <ul class="cd-hri-nav cd-hri-nav--primary">
              <?php print render($main_menu_dropdown); ?>
            </ul>
          </nav>
        </div>
      </div>
    </div>
  </header>

  <!-- Logo for printed pages -->
  <div class="visible-print-block pull-right">
    <?php if ($logo): ?>
      <img src="<?php print $logo; ?>" alt="Humanitarianresponse Logo" />
    <?php endif; ?>
  </div>

  <div id="main-wrapper">
    <div id="main" class="main">
      <div class="container">
        <?php if ($messages): ?>
          <div id="messages">
            <?php print $messages; ?>
          </div>
        <?php endif; ?>
        <?php if (!empty($page['sidebar_first'])): ?>
          <aside id="sidebar-first" class="col-md-3 hidden-print" role="complementary">
            <?php print render($page['sidebar_first']); ?>
          </aside>
        <?php endif; ?>
        <div id="content-wrapper" <?php if(!empty($page['sidebar_first'])) print 'class="col-md-9"'; ?>>
          <div id="page-header">
            <?php if ($title): ?>
              <div class="page-header">
                <h1 class="title"><?php print $title; ?></h1>
              </div>
            <?php endif; ?>
            <?php if ($tabs): ?>
              <div class="tabs">
                <?php print render($tabs); ?>
              </div>
            <?php endif; ?>
            <?php if ($action_links): ?>
              <ul class="action-links">
                <?php print render($action_links); ?>
              </ul>
            <?php endif; ?>
          </div>
          <div id="content" class="col-md-12">
            <?php print render($page['content']); ?>
          </div>
        </div><!-- /#content-wrapper -->
      </div><!-- #container -->
    </div> <!-- /#main -->
  </div> <!-- /#main-wrapper -->

  <div id="root_footer"></div>
</div><!-- #root -->

<footer id="footer" class="footer hidden-print" role="footer">
  <div class="container">
    <div id="footer-first" class="col-md-5">
      <p><?php print t('Service provided by'); ?><a href="http://www.unocha.org" target="_blank"><img width="200" alt="OCHA logo" src="/sites/all/themes/humanitarianresponse/assets/images/OCHA-logoWhite-22.svg"></a></p>
    </div>
    <div id="footer-second" class="col-md-3">
      
    </div>
    <div id = "footer-third" class="col-md-4">
      <p><?php print t('Except where otherwise noted, content on this site is licensed under a <a href="@creativecommons">Creative Commons Attribution 4.0</a> International license.', array(
      '@creativecommons' => 'https://creativecommons.org/licenses/by/4.0/'))?></p>
    </div>
  </div>
</footer>
