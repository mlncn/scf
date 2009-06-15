<?php
// $Id$

/**
 * @file
 * Holds a straight dump of an exported view.
 */
$view = new view;
$view->name = 'entrez_biblio_moderate';
$view->description = 'Moderation of Bibliography Imports';
$view->tag = 'entrez';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('fields', array(
  'biblio_authors' => array(
      'label' => 'Authors',
          'alter' => array(
                'alter_text' => 0,
                      'text' => '',
                            'make_link' => 0,
                                  'path' => '',
                                        'alt' => '',
                                              'prefix' => '',
                                                    'suffix' => '',
                                                          'help' => '',
                                                                'trim' => 0,
                                                                      'max_length' => '',
                                                                            'word_boundary' => 1,
                                                                                  'ellipsis' => 1,
                                                                                        'strip_tags' => 0,
                                                                                              'html' => 0,
                                                                                                  ),
                                                                                                      'biblio_label' => 1,
                                                                                                          'style_name' => 'vancouver',
                                                                                                              'exclude' => 0,
                                                                                                                  'id' => 'biblio_authors',
                                                                                                                      'table' => 'biblio',
                                                                                                                          'field' => 'biblio_authors',
                                                                                                                              'relationship' => 'none',
                                                                                                                                ),
                                                                                                                                  'title' => array(
                                                                                                                                      'label' => 'Title',
                                                                                                                                          'alter' => array(
                                                                                                                                                'alter_text' => 0,
                                                                                                                                                      'text' => '',
                                                                                                                                                            'make_link' => 0,
                                                                                                                                                                  'path' => '',
                                                                                                                                                                        'alt' => '',
                                                                                                                                                                              'prefix' => '',
                                                                                                                                                                                    'suffix' => '',
                                                                                                                                                                                          'help' => '',
                                                                                                                                                                                                'trim' => 0,
                                                                                                                                                                                                      'max_length' => '',
                                                                                                                                                                                                            'word_boundary' => 1,
                                                                                                                                                                                                                  'ellipsis' => 1,
                                                                                                                                                                                                                        'html' => 0,
                                                                                                                                                                                                                            ),
                                                                                                                                                                                                                                'link_to_node' => 1,
                                                                                                                                                                                                                                    'exclude' => 0,
                                                                                                                                                                                                                                        'id' => 'title',
                                                                                                                                                                                                                                            'table' => 'node',
                                                                                                                                                                                                                                                'field' => 'title',
                                                                                                                                                                                                                                                    'relationship' => 'none',
                                                                                                                                                                                                                                                      ),
                                                                                                                                                                                                                                                        'biblio_year' => array(
                                                                                                                                                                                                                                                            'id' => 'biblio_year',
                                                                                                                                                                                                                                                                'table' => 'biblio',
                                                                                                                                                                                                                                                                    'field' => 'biblio_year',
                                                                                                                                                                                                                                                                      ),
                                                                                                                                                                                                                                                                        'created' => array(
                                                                                                                                                                                                                                                                            'label' => 'Imported date',
                                                                                                                                                                                                                                                                                'alter' => array(
                                                                                                                                                                                                                                                                                      'alter_text' => 0,
                                                                                                                                                                                                                                                                                            'text' => '',
                                                                                                                                                                                                                                                                                                  'make_link' => 0,
                                                                                                                                                                                                                                                                                                        'path' => '',
                                                                                                                                                                                                                                                                                                              'alt' => '',
                                                                                                                                                                                                                                                                                                                    'prefix' => '',
                                                                                                                                                                                                                                                                                                                          'suffix' => '',
                                                                                                                                                                                                                                                                                                                                'help' => '',
                                                                                                                                                                                                                                                                                                                                      'trim' => 0,
                                                                                                                                                                                                                                                                                                                                            'max_length' => '',
                                                                                                                                                                                                                                                                                                                                                  'word_boundary' => 1,
                                                                                                                                                                                                                                                                                                                                                        'ellipsis' => 1,
                                                                                                                                                                                                                                                                                                                                                              'strip_tags' => 0,
                                                                                                                                                                                                                                                                                                                                                                    'html' => 0,
                                                                                                                                                                                                                                                                                                                                                                        ),
                                                                                                                                                                                                                                                                                                                                                                            'date_format' => 'small',
                                                                                                                                                                                                                                                                                                                                                                                'custom_date_format' => '',
                                                                                                                                                                                                                                                                                                                                                                                    'exclude' => 0,
                                                                                                                                                                                                                                                                                                                                                                                        'id' => 'created',
                                                                                                                                                                                                                                                                                                                                                                                            'table' => 'node',
                                                                                                                                                                                                                                                                                                                                                                                                'field' => 'created',
                                                                                                                                                                                                                                                                                                                                                                                                    'relationship' => 'none',
                                                                                                                                                                                                                                                                                                                                                                                                      ),
                                                                                                                                                                                                                                                                                                                                                                                                        'biblio_citekey' => array(
                                                                                                                                                                                                                                                                                                                                                                                                            'label' => 'Citation Key',
                                                                                                                                                                                                                                                                                                                                                                                                                'alter' => array(
                                                                                                                                                                                                                                                                                                                                                                                                                      'alter_text' => 0,
                                                                                                                                                                                                                                                                                                                                                                                                                            'text' => '',
                                                                                                                                                                                                                                                                                                                                                                                                                                  'make_link' => 0,
                                                                                                                                                                                                                                                                                                                                                                                                                                        'path' => '',
                                                                                                                                                                                                                                                                                                                                                                                                                                              'alt' => '',
                                                                                                                                                                                                                                                                                                                                                                                                                                                    'prefix' => '',
                                                                                                                                                                                                                                                                                                                                                                                                                                                          'suffix' => '',
                                                                                                                                                                                                                                                                                                                                                                                                                                                                'help' => '',
                                                                                                                                                                                                                                                                                                                                                                                                                                                                      'trim' => 0,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                            'max_length' => '',
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  'word_boundary' => 1,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        'ellipsis' => 1,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              'strip_tags' => 0,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    'html' => 0,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        ),
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            'biblio_label' => 1,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                'exclude' => 0,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    'id' => 'biblio_citekey',
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        'table' => 'biblio',
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            'field' => 'biblio_citekey',
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                'relationship' => 'none',
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  ),
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  ));
$handler->override_option('filters', array(
  'type' => array(
      'operator' => 'in',
          'value' => array(
                'biblio' => 'biblio',
                    ),
                        'group' => '0',
                            'exposed' => FALSE,
                                'expose' => array(
                                      'operator' => FALSE,
                                            'label' => '',
                                                ),
                                                    'id' => 'type',
                                                        'table' => 'node',
                                                            'field' => 'type',
                                                                'relationship' => 'none',
                                                                  ),
                                                                  ));
$handler->override_option('access', array(
  'type' => 'perm',
    'perm' => 'administer biblio',
    ));
$handler->override_option('title', 'Bulk-imported Bibliographies');
$handler->override_option('header_format', '2');
$handler->override_option('header_empty', 1);
$handler->override_option('items_per_page', 30);
$handler->override_option('use_pager', 'mini');
$handler->override_option('style_plugin', 'bulk');
$handler->override_option('style_options', array(
  'grouping' => '',
    'override' => 1,
      'sticky' => 0,
        'order' => 'desc',
          'columns' => array(
              'title' => 'title',
                  'created' => 'created',
                      'changed' => 'changed',
                          'title_1' => 'title_1',
                              'ops' => 'ops',
                                  'ops_1' => 'ops_1',
                                    ),
                                      'info' => array(
                                          'title' => array(
                                                'sortable' => 1,
                                                      'separator' => '',
                                                          ),
                                                              'created' => array(
                                                                    'sortable' => 1,
                                                                          'separator' => '',
                                                                              ),
                                                                                  'changed' => array(
                                                                                        'sortable' => 1,
                                                                                              'separator' => '',
                                                                                                  ),
                                                                                                      'title_1' => array(
                                                                                                            'sortable' => 1,
                                                                                                                  'separator' => '',
                                                                                                                      ),
                                                                                                                          'ops' => array(
                                                                                                                                'separator' => '',
                                                                                                                                    ),
                                                                                                                                        'ops_1' => array(
                                                                                                                                              'separator' => '',
                                                                                                                                                  ),
                                                                                                                                                    ),
                                                                                                                                                      'default' => 'created',
                                                                                                                                                        'execution_type' => '1',
                                                                                                                                                          'display_type' => '0',
                                                                                                                                                            'skip_confirmation' => 1,
                                                                                                                                                              'display_result' => 0,
                                                                                                                                                                'merge_single_action' => 0,
                                                                                                                                                                  'selected_operations' => array(
                                                                                                                                                                      '3864faba61f46e2b68393b7fc88fa830' => 0,
                                                                                                                                                                          '6f7871c7725f0c91c07594602f07aa72' => 0,
                                                                                                                                                                              '10edeb21cdd1fd049751311229c5e736' => 0,
                                                                                                                                                                                  '7a3d2d5b1bb6ecb0cb811c4130452d7d' => 0,
                                                                                                                                                                                      '591b41a4f10db44fb0b09b1e25152cef' => 0,
                                                                                                                                                                                          '1dcecde02b48fdfde9628fea88920225' => 0,
                                                                                                                                                                                              '3b3573187a8f60eecaac859273a651c1' => 0,
                                                                                                                                                                                                  '1e97c6ba8028058a3a5657fbe6a6b097' => 0,
                                                                                                                                                                                                      '78590ec12ae326a290fe6c6f4ea9ff9f' => 0,
                                                                                                                                                                                                          '44d0efe6b13ac9d05a39efa7020c58e5' => 0,
                                                                                                                                                                                                              '7043ea89c02b2b503bd9aaf95a662bdd' => 0,
                                                                                                                                                                                                                  '468f38bf30743a29f8a3214c47921b81' => 0,
                                                                                                                                                                                                                      '04fc237d7d5e286e3e725163ec88a8c1' => 0,
                                                                                                                                                                                                                          'a7fdf1558dff507d3d2d556feab826ec' => 0,
                                                                                                                                                                                                                              '0a6fda4e5fac8bb80fc2584a7ea06895' => 0,
                                                                                                                                                                                                                                  '8c3e177c3c3492b30cdabd597eff5756' => 0,
                                                                                                                                                                                                                                      'd39fd59c16bd163a9b563823a96376c7' => 0,
                                                                                                                                                                                                                                          '5471472ae2eedc59c992b6859d26033b' => 0,
                                                                                                                                                                                                                                              '95680c8f47cd9a55d448b6b7866240f1' => 0,
                                                                                                                                                                                                                                                  'c6d3850086dc3c1bca4bf0625ce78c56' => 0,
                                                                                                                                                                                                                                                      '548749a939aa6334157edff3d8f69351' => 0,
                                                                                                                                                                                                                                                          '799e8527bbd3d99e4c8c25a8a674d264' => 0,
                                                                                                                                                                                                                                                              'fce859fcce0e81943c3e31c20ccd68ff' => 0,
                                                                                                                                                                                                                                                                  '0341fcc505281031474b68c72e03a997' => 0,
                                                                                                                                                                                                                                                                      '94a098afb1689f950d72ed4c44f8655d' => 0,
                                                                                                                                                                                                                                                                          '6a99179e9c90e9edfb34d075fa234d78' => 0,
                                                                                                                                                                                                                                                                              '15faf15d7f591f8ef8e80238dd7c8b8c' => 0,
                                                                                                                                                                                                                                                                                  'c6db6d4ee28924020b504a5feb2bff9f' => 0,
                                                                                                                                                                                                                                                                                      '132fddcb3f367a243bee632db31985c1' => 0,
                                                                                                                                                                                                                                                                                          'daa75f478e3093ab107e657da6620a91' => 0,
                                                                                                                                                                                                                                                                                              '52aec7fee2070ce530da1304653ae1ec' => 0,
                                                                                                                                                                                                                                                                                                  '4df6cb87c9bbb08ebec8c757e9a0fcc7' => 0,
                                                                                                                                                                                                                                                                                                      'f033f40d92f0308ed6b2e3befb3a4c2a' => 0,
                                                                                                                                                                                                                                                                                                          '334d20af1ae7ac4b770b7ec3210b2638' => 0,
                                                                                                                                                                                                                                                                                                              '3ea33dae73a849deedd536991455be98' => 0,
                                                                                                                                                                                                                                                                                                                  'feb13e750bd2575b1f36109233087905' => 0,
                                                                                                                                                                                                                                                                                                                      '2178a36c0b51f3a7ea1d854780e33cc5' => 0,
                                                                                                                                                                                                                                                                                                                          '246fdc2a4672eb371d05b48b2a7cb51e' => 0,
                                                                                                                                                                                                                                                                                                                              '063bfcd6b7c1fa6a81515afb705fdbb7' => 0,
                                                                                                                                                                                                                                                                                                                                  'd2856892e9560840d2fbbfc63336e221' => 0,
                                                                                                                                                                                                                                                                                                                                      '4c25595683f62f9f9dce8cc57be82c81' => 0,
                                                                                                                                                                                                                                                                                                                                          '72f421760d21f9b73c0131a3aff5b6e5' => 0,
                                                                                                                                                                                                                                                                                                                                              '0b73284699abf99316be5e7e2ab93e8a' => 0,
                                                                                                                                                                                                                                                                                                                                                ),
                                                                                                                                                                                                                                                                                                                                                ));
$handler = $view->new_display('page', 'Published/Approved', 'page_1');
$handler->override_option('filters', array(
  'status' => array(
      'operator' => '=',
          'value' => '1',
              'group' => '0',
                  'exposed' => FALSE,
                      'expose' => array(
                            'operator' => FALSE,
                                  'label' => '',
                                      ),
                                          'id' => 'status',
                                              'table' => 'node',
                                                  'field' => 'status',
                                                      'relationship' => 'none',
                                                        ),
                                                          'type' => array(
                                                              'operator' => 'in',
                                                                  'value' => array(
                                                                        'biblio' => 'biblio',
                                                                            ),
                                                                                'group' => '0',
                                                                                    'exposed' => FALSE,
                                                                                        'expose' => array(
                                                                                              'operator' => FALSE,
                                                                                                    'label' => '',
                                                                                                        ),
                                                                                                            'id' => 'type',
                                                                                                                'table' => 'node',
                                                                                                                    'field' => 'type',
                                                                                                                        'override' => array(
                                                                                                                              'button' => 'Use default',
                                                                                                                                  ),
                                                                                                                                      'relationship' => 'none',
                                                                                                                                        ),
                                                                                                                                        ));
$handler->override_option('title', 'Published bibliographies');
$handler->override_option('empty', 'No bibliographies have been approved for publishing yet.
');
$handler->override_option('empty_format', '1');
$handler->override_option('style_options', array(
  'grouping' => '',
    'override' => 1,
      'sticky' => 0,
        'order' => 'desc',
          'columns' => array(
              'biblio_authors' => 'biblio_authors',
                  'title' => 'title',
                      'biblio_year' => 'biblio_year',
                          'created' => 'created',
                              'biblio_citekey' => 'biblio_citekey',
                                ),
                                  'info' => array(
                                      'biblio_authors' => array(
                                            'separator' => '',
                                                ),
                                                    'title' => array(
                                                          'sortable' => 1,
                                                                'separator' => '',
                                                                    ),
                                                                        'biblio_year' => array(
                                                                              'separator' => '',
                                                                                  ),
                                                                                      'created' => array(
                                                                                            'sortable' => 1,
                                                                                                  'separator' => '',
                                                                                                      ),
                                                                                                          'biblio_citekey' => array(
                                                                                                                'separator' => '',
                                                                                                                    ),
                                                                                                                      ),
                                                                                                                        'default' => 'created',
                                                                                                                          'execution_type' => '1',
                                                                                                                            'display_type' => '1',
                                                                                                                              'skip_confirmation' => 1,
                                                                                                                                'display_result' => 0,
                                                                                                                                  'merge_single_action' => 0,
                                                                                                                                    'selected_operations' => array(
                                                                                                                                        '09fe7b4f6155ed31c3593afcef29305f' => '09fe7b4f6155ed31c3593afcef29305f',
                                                                                                                                            '35fd0346ecbfb49bfefa7dba78295890' => 0,
                                                                                                                                                '8c3e177c3c3492b30cdabd597eff5756' => 0,
                                                                                                                                                    '4c25595683f62f9f9dce8cc57be82c81' => 0,
                                                                                                                                                        '3b3573187a8f60eecaac859273a651c1' => 0,
                                                                                                                                                            '6a99179e9c90e9edfb34d075fa234d78' => 0,
                                                                                                                                                                '0b73284699abf99316be5e7e2ab93e8a' => 0,
                                                                                                                                                                    'd2856892e9560840d2fbbfc63336e221' => 0,
                                                                                                                                                                        '063bfcd6b7c1fa6a81515afb705fdbb7' => 0,
                                                                                                                                                                            '3ea33dae73a849deedd536991455be98' => 0,
                                                                                                                                                                                'a7fdf1558dff507d3d2d556feab826ec' => 0,
                                                                                                                                                                                    '7a3d2d5b1bb6ecb0cb811c4130452d7d' => 0,
                                                                                                                                                                                        '132fddcb3f367a243bee632db31985c1' => 0,
                                                                                                                                                                                            'feb13e750bd2575b1f36109233087905' => 0,
                                                                                                                                                                                                '334d20af1ae7ac4b770b7ec3210b2638' => 0,
                                                                                                                                                                                                    '799e8527bbd3d99e4c8c25a8a674d264' => 0,
                                                                                                                                                                                                        'fce859fcce0e81943c3e31c20ccd68ff' => 0,
                                                                                                                                                                                                            '591b41a4f10db44fb0b09b1e25152cef' => 0,
                                                                                                                                                                                                                '78590ec12ae326a290fe6c6f4ea9ff9f' => 0,
                                                                                                                                                                                                                    '4df6cb87c9bbb08ebec8c757e9a0fcc7' => 0,
                                                                                                                                                                                                                        'f033f40d92f0308ed6b2e3befb3a4c2a' => 0,
                                                                                                                                                                                                                            '0a6fda4e5fac8bb80fc2584a7ea06895' => 0,
                                                                                                                                                                                                                                '468f38bf30743a29f8a3214c47921b81' => 0,
                                                                                                                                                                                                                                    '9b08a2a85f208e546236adb08fee597c' => 0,
                                                                                                                                                                                                                                        '04fc237d7d5e286e3e725163ec88a8c1' => 0,
                                                                                                                                                                                                                                            '0341fcc505281031474b68c72e03a997' => 0,
                                                                                                                                                                                                                                                '10edeb21cdd1fd049751311229c5e736' => 0,
                                                                                                                                                                                                                                                    '3864faba61f46e2b68393b7fc88fa830' => 0,
                                                                                                                                                                                                                                                        '56ab37433d5a0cc0464ab32f6b50ba7e' => 0,
                                                                                                                                                                                                                                                            'c6d3850086dc3c1bca4bf0625ce78c56' => 0,
                                                                                                                                                                                                                                                                'd39fd59c16bd163a9b563823a96376c7' => 0,
                                                                                                                                                                                                                                                                    '246fdc2a4672eb371d05b48b2a7cb51e' => 0,
                                                                                                                                                                                                                                                                        '52aec7fee2070ce530da1304653ae1ec' => 0,
                                                                                                                                                                                                                                                                            '7043ea89c02b2b503bd9aaf95a662bdd' => 0,
                                                                                                                                                                                                                                                                                '44d0efe6b13ac9d05a39efa7020c58e5' => 0,
                                                                                                                                                                                                                                                                                    '2b9393b809ddde9e0a182ad64230afb5' => 0,
                                                                                                                                                                                                                                                                                        '94a098afb1689f950d72ed4c44f8655d' => 0,
                                                                                                                                                                                                                                                                                            '1dcecde02b48fdfde9628fea88920225' => 0,
                                                                                                                                                                                                                                                                                                '15faf15d7f591f8ef8e80238dd7c8b8c' => 0,
                                                                                                                                                                                                                                                                                                    'daa75f478e3093ab107e657da6620a91' => 0,
                                                                                                                                                                                                                                                                                                        '2178a36c0b51f3a7ea1d854780e33cc5' => 0,
                                                                                                                                                                                                                                                                                                            '72f421760d21f9b73c0131a3aff5b6e5' => 0,
                                                                                                                                                                                                                                                                                                                '1e97c6ba8028058a3a5657fbe6a6b097' => 0,
                                                                                                                                                                                                                                                                                                                    '6f7871c7725f0c91c07594602f07aa72' => 0,
                                                                                                                                                                                                                                                                                                                        '548749a939aa6334157edff3d8f69351' => 0,
                                                                                                                                                                                                                                                                                                                            'c6db6d4ee28924020b504a5feb2bff9f' => 0,
                                                                                                                                                                                                                                                                                                                                '95680c8f47cd9a55d448b6b7866240f1' => 0,
                                                                                                                                                                                                                                                                                                                                    '5471472ae2eedc59c992b6859d26033b' => 0,
                                                                                                                                                                                                                                                                                                                                      ),
                                                                                                                                                                                                                                                                                                                                      ));
$handler->override_option('path', 'admin/content/entrez/biblio-moderate/published');
$handler->override_option('menu', array(
  'type' => 'tab',
    'title' => 'Approved',
      'description' => '',
        'weight' => '0',
          'name' => 'navigation',
          ));
$handler->override_option('tab_options', array(
  'type' => 'none',
    'title' => '',
      'description' => '',
        'weight' => 0,
        ));
$handler = $view->new_display('page', 'Unpublished/Unmoderated', 'page_3');
$handler->override_option('relationships', array(
  'flag_content_rel' => array(
      'label' => 'Removed from Published Biblios',
          'required' => 0,
              'flag' => 'entrez_biblio_removed',
                  'user_scope' => 'any',
                      'id' => 'flag_content_rel',
                          'table' => 'node',
                              'field' => 'flag_content_rel',
                                  'override' => array(
                                        'button' => 'Use default',
                                            ),
                                                'relationship' => 'none',
                                                  ),
                                                  ));
$handler->override_option('filters', array(
  'status' => array(
      'operator' => '=',
          'value' => '0',
              'group' => '0',
                  'exposed' => FALSE,
                      'expose' => array(
                            'operator' => FALSE,
                                  'label' => '',
                                      ),
                                          'id' => 'status',
                                              'table' => 'node',
                                                  'field' => 'status',
                                                      'relationship' => 'none',
                                                          'override' => array(
                                                                'button' => 'Use default',
                                                                    ),
                                                                      ),
                                                                        'type' => array(
                                                                            'operator' => 'in',
                                                                                'value' => array(
                                                                                      'biblio' => 'biblio',
                                                                                          ),
                                                                                              'group' => '0',
                                                                                                  'exposed' => FALSE,
                                                                                                      'expose' => array(
                                                                                                            'operator' => FALSE,
                                                                                                                  'label' => '',
                                                                                                                      ),
                                                                                                                          'id' => 'type',
                                                                                                                              'table' => 'node',
                                                                                                                                  'field' => 'type',
                                                                                                                                      'override' => array(
                                                                                                                                            'button' => 'Use default',
                                                                                                                                                ),
                                                                                                                                                    'relationship' => 'none',
                                                                                                                                                      ),
                                                                                                                                                        'flagged' => array(
                                                                                                                                                            'operator' => '=',
                                                                                                                                                                'value' => '0',
                                                                                                                                                                    'group' => '0',
                                                                                                                                                                        'exposed' => FALSE,
                                                                                                                                                                            'expose' => array(
                                                                                                                                                                                  'operator' => FALSE,
                                                                                                                                                                                        'label' => '',
                                                                                                                                                                                            ),
                                                                                                                                                                                                'id' => 'flagged',
                                                                                                                                                                                                    'table' => 'flag_content',
                                                                                                                                                                                                        'field' => 'flagged',
                                                                                                                                                                                                            'relationship' => 'flag_content_rel',
                                                                                                                                                                                                                'override' => array(
                                                                                                                                                                                                                      'button' => 'Use default',
                                                                                                                                                                                                                          ),
                                                                                                                                                                                                                            ),
                                                                                                                                                                                                                            ));
$handler->override_option('title', 'Biblios from bulk import awaiting moderation');
$handler->override_option('empty', 'No bulk-imported bibliographies need to be moderated.');
$handler->override_option('empty_format', '1');
$handler->override_option('style_options', array(
  'grouping' => '',
    'override' => 1,
      'sticky' => 0,
        'order' => 'desc',
          'columns' => array(
              'biblio_authors' => 'biblio_authors',
                  'title' => 'title',
                      'biblio_year' => 'biblio_year',
                          'created' => 'created',
                              'biblio_citekey' => 'biblio_citekey',
                                ),
                                  'info' => array(
                                      'biblio_authors' => array(
                                            'separator' => '',
                                                ),
                                                    'title' => array(
                                                          'sortable' => 1,
                                                                'separator' => '',
                                                                    ),
                                                                        'biblio_year' => array(
                                                                              'separator' => '',
                                                                                  ),
                                                                                      'created' => array(
                                                                                            'sortable' => 1,
                                                                                                  'separator' => '',
                                                                                                      ),
                                                                                                          'biblio_citekey' => array(
                                                                                                                'separator' => '',
                                                                                                                    ),
                                                                                                                      ),
                                                                                                                        'default' => 'created',
                                                                                                                          'execution_type' => '1',
                                                                                                                            'display_type' => '1',
                                                                                                                              'skip_confirmation' => 1,
                                                                                                                                'display_result' => 0,
                                                                                                                                  'merge_single_action' => 0,
                                                                                                                                    'selected_operations' => array(
                                                                                                                                        '3864faba61f46e2b68393b7fc88fa830' => '3864faba61f46e2b68393b7fc88fa830',
                                                                                                                                            '09fe7b4f6155ed31c3593afcef29305f' => '09fe7b4f6155ed31c3593afcef29305f',
                                                                                                                                                '35fd0346ecbfb49bfefa7dba78295890' => 0,
                                                                                                                                                    '8c3e177c3c3492b30cdabd597eff5756' => 0,
                                                                                                                                                        '4c25595683f62f9f9dce8cc57be82c81' => 0,
                                                                                                                                                            '3b3573187a8f60eecaac859273a651c1' => 0,
                                                                                                                                                                '6a99179e9c90e9edfb34d075fa234d78' => 0,
                                                                                                                                                                    '0b73284699abf99316be5e7e2ab93e8a' => 0,
                                                                                                                                                                        'd2856892e9560840d2fbbfc63336e221' => 0,
                                                                                                                                                                            '063bfcd6b7c1fa6a81515afb705fdbb7' => 0,
                                                                                                                                                                                '3ea33dae73a849deedd536991455be98' => 0,
                                                                                                                                                                                    'a7fdf1558dff507d3d2d556feab826ec' => 0,
                                                                                                                                                                                        '7a3d2d5b1bb6ecb0cb811c4130452d7d' => 0,
                                                                                                                                                                                            '132fddcb3f367a243bee632db31985c1' => 0,
                                                                                                                                                                                                'feb13e750bd2575b1f36109233087905' => 0,
                                                                                                                                                                                                    '334d20af1ae7ac4b770b7ec3210b2638' => 0,
                                                                                                                                                                                                        '799e8527bbd3d99e4c8c25a8a674d264' => 0,
                                                                                                                                                                                                            'fce859fcce0e81943c3e31c20ccd68ff' => 0,
                                                                                                                                                                                                                '591b41a4f10db44fb0b09b1e25152cef' => 0,
                                                                                                                                                                                                                    '78590ec12ae326a290fe6c6f4ea9ff9f' => 0,
                                                                                                                                                                                                                        '4df6cb87c9bbb08ebec8c757e9a0fcc7' => 0,
                                                                                                                                                                                                                            'f033f40d92f0308ed6b2e3befb3a4c2a' => 0,
                                                                                                                                                                                                                                '0a6fda4e5fac8bb80fc2584a7ea06895' => 0,
                                                                                                                                                                                                                                    '468f38bf30743a29f8a3214c47921b81' => 0,
                                                                                                                                                                                                                                        '9b08a2a85f208e546236adb08fee597c' => 0,
                                                                                                                                                                                                                                            '04fc237d7d5e286e3e725163ec88a8c1' => 0,
                                                                                                                                                                                                                                                '0341fcc505281031474b68c72e03a997' => 0,
                                                                                                                                                                                                                                                    '10edeb21cdd1fd049751311229c5e736' => 0,
                                                                                                                                                                                                                                                        '56ab37433d5a0cc0464ab32f6b50ba7e' => 0,
                                                                                                                                                                                                                                                            'c6d3850086dc3c1bca4bf0625ce78c56' => 0,
                                                                                                                                                                                                                                                                'd39fd59c16bd163a9b563823a96376c7' => 0,
                                                                                                                                                                                                                                                                    '246fdc2a4672eb371d05b48b2a7cb51e' => 0,
                                                                                                                                                                                                                                                                        '52aec7fee2070ce530da1304653ae1ec' => 0,
                                                                                                                                                                                                                                                                            '7043ea89c02b2b503bd9aaf95a662bdd' => 0,
                                                                                                                                                                                                                                                                                '44d0efe6b13ac9d05a39efa7020c58e5' => 0,
                                                                                                                                                                                                                                                                                    '2b9393b809ddde9e0a182ad64230afb5' => 0,
                                                                                                                                                                                                                                                                                        '94a098afb1689f950d72ed4c44f8655d' => 0,
                                                                                                                                                                                                                                                                                            '1dcecde02b48fdfde9628fea88920225' => 0,
                                                                                                                                                                                                                                                                                                '15faf15d7f591f8ef8e80238dd7c8b8c' => 0,
                                                                                                                                                                                                                                                                                                    'daa75f478e3093ab107e657da6620a91' => 0,
                                                                                                                                                                                                                                                                                                        '2178a36c0b51f3a7ea1d854780e33cc5' => 0,
                                                                                                                                                                                                                                                                                                            '72f421760d21f9b73c0131a3aff5b6e5' => 0,
                                                                                                                                                                                                                                                                                                                '1e97c6ba8028058a3a5657fbe6a6b097' => 0,
                                                                                                                                                                                                                                                                                                                    '6f7871c7725f0c91c07594602f07aa72' => 0,
                                                                                                                                                                                                                                                                                                                        '548749a939aa6334157edff3d8f69351' => 0,
                                                                                                                                                                                                                                                                                                                            'c6db6d4ee28924020b504a5feb2bff9f' => 0,
                                                                                                                                                                                                                                                                                                                                '95680c8f47cd9a55d448b6b7866240f1' => 0,
                                                                                                                                                                                                                                                                                                                                    '5471472ae2eedc59c992b6859d26033b' => 0,
                                                                                                                                                                                                                                                                                                                                      ),
                                                                                                                                                                                                                                                                                                                                      ));
$handler->override_option('path', 'admin/content/entrez/biblio-moderate/unpublished');
$handler->override_option('menu', array(
  'type' => 'default tab',
    'title' => 'Awaiting Moderation',
      'description' => 'Bulk-imported bibliographies awaiting approval or rejection.',
        'weight' => '-6',
          'name' => 'navigation',
          ));
$handler->override_option('tab_options', array(
  'type' => 'tab',
    'title' => 'Moderate biblios',
      'description' => 'Publish or permanently reject bulk-imported bibliographies.',
        'weight' => '-6',
        ));
$handler = $view->new_display('page', 'Removed Biblios', 'page_4');
$handler->override_option('relationships', array(
  'flag_content_rel' => array(
      'label' => 'entrez_biblio_removed_flag',
          'required' => 1,
              'flag' => 'entrez_biblio_removed',
                  'user_scope' => 'any',
                      'id' => 'flag_content_rel',
                          'table' => 'node',
                              'field' => 'flag_content_rel',
                                  'override' => array(
                                        'button' => 'Use default',
                                            ),
                                                'relationship' => 'none',
                                                  ),
                                                  ));
$handler->override_option('filters', array(
  'status' => array(
      'operator' => '=',
          'value' => '0',
              'group' => '0',
                  'exposed' => FALSE,
                      'expose' => array(
                            'operator' => FALSE,
                                  'label' => '',
                                      ),
                                          'id' => 'status',
                                              'table' => 'node',
                                                  'field' => 'status',
                                                      'relationship' => 'none',
                                                          'override' => array(
                                                                'button' => 'Use default',
                                                                    ),
                                                                      ),
                                                                        'type' => array(
                                                                            'operator' => 'in',
                                                                                'value' => array(
                                                                                      'biblio' => 'biblio',
                                                                                          ),
                                                                                              'group' => '0',
                                                                                                  'exposed' => FALSE,
                                                                                                      'expose' => array(
                                                                                                            'operator' => FALSE,
                                                                                                                  'label' => '',
                                                                                                                      ),
                                                                                                                          'id' => 'type',
                                                                                                                              'table' => 'node',
                                                                                                                                  'field' => 'type',
                                                                                                                                      'override' => array(
                                                                                                                                            'button' => 'Use default',
                                                                                                                                                ),
                                                                                                                                                    'relationship' => 'none',
                                                                                                                                                      ),
                                                                                                                                                        'flagged' => array(
                                                                                                                                                            'operator' => '=',
                                                                                                                                                                'value' => '1',
                                                                                                                                                                    'group' => '0',
                                                                                                                                                                        'exposed' => FALSE,
                                                                                                                                                                            'expose' => array(
                                                                                                                                                                                  'operator' => FALSE,
                                                                                                                                                                                        'label' => '',
                                                                                                                                                                                            ),
                                                                                                                                                                                                'id' => 'flagged',
                                                                                                                                                                                                    'table' => 'flag_content',
                                                                                                                                                                                                        'field' => 'flagged',
                                                                                                                                                                                                            'relationship' => 'flag_content_rel',
                                                                                                                                                                                                                'override' => array(
                                                                                                                                                                                                                      'button' => 'Use default',
                                                                                                                                                                                                                          ),
                                                                                                                                                                                                                            ),
                                                                                                                                                                                                                            ));
$handler->override_option('title', 'Rejected bibliographies (removed from future imports)');
$handler->override_option('empty', 'No bibliographies have been permanently rejected from publishing or future import.');
$handler->override_option('empty_format', '1');
$handler->override_option('style_options', array(
  'grouping' => '',
    'override' => 1,
      'sticky' => 0,
        'order' => 'desc',
          'columns' => array(
              'biblio_authors' => 'biblio_authors',
                  'title' => 'title',
                      'biblio_year' => 'biblio_year',
                          'created' => 'created',
                              'changed' => 'changed',
                                  'biblio_citekey' => 'biblio_citekey',
                                    ),
                                      'info' => array(
                                          'biblio_authors' => array(
                                                'separator' => '',
                                                    ),
                                                        'title' => array(
                                                              'sortable' => 1,
                                                                    'separator' => '',
                                                                        ),
                                                                            'biblio_year' => array(
                                                                                  'separator' => '',
                                                                                      ),
                                                                                          'created' => array(
                                                                                                'sortable' => 1,
                                                                                                      'separator' => '',
                                                                                                          ),
                                                                                                              'changed' => array(
                                                                                                                    'sortable' => 1,
                                                                                                                          'separator' => '',
                                                                                                                              ),
                                                                                                                                  'biblio_citekey' => array(
                                                                                                                                        'separator' => '',
                                                                                                                                            ),
                                                                                                                                              ),
                                                                                                                                                'default' => 'created',
                                                                                                                                                  'execution_type' => '1',
                                                                                                                                                    'display_type' => '1',
                                                                                                                                                      'skip_confirmation' => 1,
                                                                                                                                                        'display_result' => 0,
                                                                                                                                                          'merge_single_action' => 0,
                                                                                                                                                            'selected_operations' => array(
                                                                                                                                                                '56ab37433d5a0cc0464ab32f6b50ba7e' => '56ab37433d5a0cc0464ab32f6b50ba7e',
                                                                                                                                                                    '35fd0346ecbfb49bfefa7dba78295890' => 0,
                                                                                                                                                                        '8c3e177c3c3492b30cdabd597eff5756' => 0,
                                                                                                                                                                            '4c25595683f62f9f9dce8cc57be82c81' => 0,
                                                                                                                                                                                '3b3573187a8f60eecaac859273a651c1' => 0,
                                                                                                                                                                                    '6a99179e9c90e9edfb34d075fa234d78' => 0,
                                                                                                                                                                                        '0b73284699abf99316be5e7e2ab93e8a' => 0,
                                                                                                                                                                                            'd2856892e9560840d2fbbfc63336e221' => 0,
                                                                                                                                                                                                '063bfcd6b7c1fa6a81515afb705fdbb7' => 0,
                                                                                                                                                                                                    '3ea33dae73a849deedd536991455be98' => 0,
                                                                                                                                                                                                        'a7fdf1558dff507d3d2d556feab826ec' => 0,
                                                                                                                                                                                                            '7a3d2d5b1bb6ecb0cb811c4130452d7d' => 0,
                                                                                                                                                                                                                '132fddcb3f367a243bee632db31985c1' => 0,
                                                                                                                                                                                                                    'feb13e750bd2575b1f36109233087905' => 0,
                                                                                                                                                                                                                        '334d20af1ae7ac4b770b7ec3210b2638' => 0,
                                                                                                                                                                                                                            '799e8527bbd3d99e4c8c25a8a674d264' => 0,
                                                                                                                                                                                                                                'fce859fcce0e81943c3e31c20ccd68ff' => 0,
                                                                                                                                                                                                                                    '591b41a4f10db44fb0b09b1e25152cef' => 0,
                                                                                                                                                                                                                                        '78590ec12ae326a290fe6c6f4ea9ff9f' => 0,
                                                                                                                                                                                                                                            '4df6cb87c9bbb08ebec8c757e9a0fcc7' => 0,
                                                                                                                                                                                                                                                'f033f40d92f0308ed6b2e3befb3a4c2a' => 0,
                                                                                                                                                                                                                                                    '0a6fda4e5fac8bb80fc2584a7ea06895' => 0,
                                                                                                                                                                                                                                                        '468f38bf30743a29f8a3214c47921b81' => 0,
                                                                                                                                                                                                                                                            '9b08a2a85f208e546236adb08fee597c' => 0,
                                                                                                                                                                                                                                                                '04fc237d7d5e286e3e725163ec88a8c1' => 0,
                                                                                                                                                                                                                                                                    '0341fcc505281031474b68c72e03a997' => 0,
                                                                                                                                                                                                                                                                        '10edeb21cdd1fd049751311229c5e736' => 0,
                                                                                                                                                                                                                                                                            '3864faba61f46e2b68393b7fc88fa830' => 0,
                                                                                                                                                                                                                                                                                'c6d3850086dc3c1bca4bf0625ce78c56' => 0,
                                                                                                                                                                                                                                                                                    'd39fd59c16bd163a9b563823a96376c7' => 0,
                                                                                                                                                                                                                                                                                        '246fdc2a4672eb371d05b48b2a7cb51e' => 0,
                                                                                                                                                                                                                                                                                            '52aec7fee2070ce530da1304653ae1ec' => 0,
                                                                                                                                                                                                                                                                                                '7043ea89c02b2b503bd9aaf95a662bdd' => 0,
                                                                                                                                                                                                                                                                                                    '09fe7b4f6155ed31c3593afcef29305f' => 0,
                                                                                                                                                                                                                                                                                                        '44d0efe6b13ac9d05a39efa7020c58e5' => 0,
                                                                                                                                                                                                                                                                                                            '2b9393b809ddde9e0a182ad64230afb5' => 0,
                                                                                                                                                                                                                                                                                                                '94a098afb1689f950d72ed4c44f8655d' => 0,
                                                                                                                                                                                                                                                                                                                    '1dcecde02b48fdfde9628fea88920225' => 0,
                                                                                                                                                                                                                                                                                                                        '15faf15d7f591f8ef8e80238dd7c8b8c' => 0,
                                                                                                                                                                                                                                                                                                                            'daa75f478e3093ab107e657da6620a91' => 0,
                                                                                                                                                                                                                                                                                                                                '2178a36c0b51f3a7ea1d854780e33cc5' => 0,
                                                                                                                                                                                                                                                                                                                                    '72f421760d21f9b73c0131a3aff5b6e5' => 0,
                                                                                                                                                                                                                                                                                                                                        '1e97c6ba8028058a3a5657fbe6a6b097' => 0,
                                                                                                                                                                                                                                                                                                                                            '6f7871c7725f0c91c07594602f07aa72' => 0,
                                                                                                                                                                                                                                                                                                                                                '548749a939aa6334157edff3d8f69351' => 0,
                                                                                                                                                                                                                                                                                                                                                    'c6db6d4ee28924020b504a5feb2bff9f' => 0,
                                                                                                                                                                                                                                                                                                                                                        '95680c8f47cd9a55d448b6b7866240f1' => 0,
                                                                                                                                                                                                                                                                                                                                                            '5471472ae2eedc59c992b6859d26033b' => 0,
                                                                                                                                                                                                                                                                                                                                                              ),
                                                                                                                                                                                                                                                                                                                                                              ));
$handler->override_option('path', 'admin/content/entrez/biblio-moderate/removed');
$handler->override_option('menu', array(
  'type' => 'tab',
    'title' => 'Rejected',
      'description' => 'Bibliographies removed from showing up on site or in approval queue.',
        'weight' => '2',
          'name' => 'navigation',
          ));
$handler->override_option('tab_options', array(
  'type' => 'none',
    'title' => '',
      'description' => '',
        'weight' => 0,
        ));

