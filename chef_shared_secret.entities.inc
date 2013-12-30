<?php

/**
 * @file
 * Entity API classes for chef_shared_secret module.
 */


/**
 * The class used for chef_shared_secret entities.
 */
class ChefSharedSecret extends Entity {

  public $name;
  public $secret;

  public function __construct($values = array()) {
    parent::__construct($values, 'chef_shared_secret');
  }

  public function hash($raw_output = FALSE) {
    return hash('sha256', $this->secret, $raw_output);
  }

  public function generate() {
    $this->secret = base64_encode(drupal_random_bytes(CHEF_SHARED_SECRET_GENERATE_LENGTH));
  }
}

/**
 * The Controller for chef_shared_secret entities.
 */
class ChefSharedSecretController extends EntityAPIControllerExportable {
  public function __construct($entityType) {
    parent::__construct($entityType);
  }

  /**
   * Create a chef_shared_secret.
   *
   * @return
   *   A chef_shared_secret object with all default fields initialized.
   */
  public function create(array $values = array()) {
    // Add values that are specific to our Model
    $values += array(
      'name' => '',
      'secret' => '',
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'data' => '',
    );
    $entity = parent::create($values);
    return $entity;
  }

  /**
   * Save a chef_shared_secret entity.
   */
  public function save($entity) {
    $entity->changed = REQUEST_TIME;
    return parent::save($entity);
  }
}
