<?php

namespace Drupal\drupalfr_user\Form;

use Drupal\Core\Config\ConfigFactoryInterface;
use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\State\StateInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Membership setting.
 *
 * @package Drupal\drupalfr_user\Form
 */
class MembershipSettings extends ConfigFormBase
{

  /**
   * {@inheritdoc}
   */
    public function getFormId()
    {
        return 'drupalfr_user_membership_settings_form';
    }

  /**
   * {@inheritdoc}
   */
    protected function getEditableConfigNames()
    {
        return [];
    }

  /**
   * The state keyvalue collection.
   *
   * @var \Drupal\Core\State\StateInterface
   */
    protected $state;

  /**
   * Constructs a MembershipSettings object.
   *
   * @param \Drupal\Core\Config\ConfigFactoryInterface $config_factory
   *   The factory for configuration objects.
   * @param \Drupal\Core\State\StateInterface $state
   *   The state keyvalue collection to use.
   */
    public function __construct(ConfigFactoryInterface $config_factory, StateInterface $state)
    {
        parent::__construct($config_factory);
        $this->state = $state;
    }

  /**
   * {@inheritdoc}
   */
    public static function create(ContainerInterface $container)
    {
        return new static(
            $container->get('config.factory'),
            $container->get('state')
        );
    }

  /**
   * {@inheritdoc}
   */
    public function buildForm(array $form, FormStateInterface $form_state)
    {
        $form['next_membership_number'] = [
        '#type' => 'number',
        '#title' => $this->t('Next membership number'),
        '#default_value' => $this->state->get('drupalfr_user.next_membership_number', 1),
        ];

        return parent::buildForm($form, $form_state);
    }

  /**
   * {@inheritdoc}
   */
    public function submitForm(array &$form, FormStateInterface $form_state)
    {
        parent::submitForm($form, $form_state);

        $this->state->set('drupalfr_user.next_membership_number', $form_state->getValue('next_membership_number'));
    }
}
