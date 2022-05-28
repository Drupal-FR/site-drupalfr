Feature: Global Elements

  @api
  Scenario: Homepage Contact Link
    Given I am on the homepage
    Then I should see the link "Home" in the "primary_menu" region
    Then I should see the link "Log in" in the "secondary_menu" region
    Then I should see the link "Contact" in the "footer_fifth" region
    Then I should see the "Search" button in the "sidebar_first" region
    Then I should see the "div#block-bartik-branding" element in the "header" region

  @api
  Scenario: Create a node
    Given I am logged in as a user with the "administrator" role
    When I am viewing an "article" content with the title "My article"
    Then I should see the heading "My article"
