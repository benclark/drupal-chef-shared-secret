<?php

/**
 * @file
 * Administrative functions and callbacks for chef_shared_secret module.
 */


/**
 * Admin UI controller for chef_shared_secret entities.
 */
class ChefSharedSecretUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage Chef shared secrets.';
    return $items;
  }
}

/**
 * Generates the chef_databag editing form.
 */
function chef_shared_secret_form($form, &$form_state, $shared_secret, $op = 'edit') {
  $form = array();

  if (module_exists('ctools')) {
    ctools_include('dependent');
  }

  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Name'),
    '#description' => t('A unique machine-readable name for this shared secret.'),
    '#required' => TRUE,
    '#default_value' => $shared_secret->name,
    '#machine_name' => array(
      'exists' => 'chef_shared_secret_exists',
    ),
    '#disabled' => ($op == 'edit'),
  );

  if (empty($shared_secret->secret) && $op != 'edit') {
    $form['do_generate'] = array(
      '#type' => 'checkbox',
      '#title' => t('Generate random shared secret string.'),
      '#description' => t('If checked, the shared secret will be randomly created.'),
      '#default_value' => TRUE,
      '#attributes' => array(
        'id' => 'chef_shared_secret_generate',
      ),
    );
  }

  $form['secret'] = array(
    '#type' => 'textarea',
    '#title' => t('Shared secret'),
    '#description' => t('A shared secret is a piece of data, known only to the parties involved, in a secure communication. The shared secret can be a password, a passphrase, a big number or an array of randomly chosen bytes. !link', array('!link' => l(t('Wikipedia article on Symmetric-key algorithms'), 'http://en.wikipedia.org/wiki/Symmetric-key_algorithm', array('absolute' => TRUE)))),
    '#required' => !isset($form['do_generate']),
    '#default_value' => $shared_secret->secret,
    '#dependency' => array('chef_shared_secret_generate' => array(FALSE)),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Form API validate callback for the chef_shared_secret form.
 */
function chef_shared_secret_form_validate(&$form, &$form_state) {
  if (isset($form_state['values']['do_generate'])) {
    if (empty($form_state['values']['do_generate']) && empty($form_state['values']['secret'])) {
      form_set_error('secret', t('Shared secret field is required when Generate field is unchecked.'));
    }
  }
}

/**
 * Form API submit callback for the chef_shared_secret form.
 */
function chef_shared_secret_form_submit(&$form, &$form_state) {
  $data_bag = entity_ui_form_submit_build_entity($form, $form_state);
  if (!empty($form_state['values']['do_generate'])) {
    unset($data_bag->do_generate);
    // Call the ChefSharedSecret::generate method.
    $data_bag->generate();
  }
  $data_bag->save();
  $form_state['redirect'] = 'admin/structure/chef_shared_secret';
}
