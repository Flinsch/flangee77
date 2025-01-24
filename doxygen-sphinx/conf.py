# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

import os
import sys

sys.path.insert(0, os.path.abspath('.'))

project = 'flangee77'
copyright = '2023-2025, Dr. Stefan "Flinsch" Fleischer'
author = 'Dr. Stefan "Flinsch" Fleischer'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    'breathe',
    'sphinx_rtd_theme',
    'sphinx.ext.coverage',
]

master_doc = 'index'

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

highlight_language = 'c++'



# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'
html_theme_options = {
    #'analytics_id': 'G-XXXXXXXXXX',  #  Provided by Google in your dashboard
    #'analytics_anonymize_ip': False,
    'logo_only': False,
    'prev_next_buttons_location': 'bottom',
    'style_external_links': True,#False,
    'vcs_pageview_mode': '',
    'style_nav_header_background': 'white',
    'flyout_display': 'hidden',
    'version_selector': False,#True,
    'language_selector': False,#True,
    # Toc options
    'collapse_navigation': True,
    'sticky_navigation': True,
    'navigation_depth': 4,
    'includehidden': True,
    'titles_only': False,
}

# A list of paths that contain custom static files (such as style sheets or script files).
# Relative paths are taken as relative to the configuration directory.
# They are copied to the output’s _static directory after the theme’s static files,
# so a file named default.css will overwrite the theme’s default.css.
#
# As these files are not meant to be built, they are automatically excluded from source files.
html_static_path = ['_static']

# The filenames must be relative to the html_static_path,
# or a full URI with scheme like 'https://example.org/style.css'.
html_css_files = [
    'custom.css',
]

# The filenames must be relative to the html_static_path,
# or a full URI with scheme like 'https://example.org/script.js'.
html_js_files = [
    'custom.js',
]



# -- Breathe configuration ---------------------------------------------------

breathe_projects = {
    'flangee77': './_build/xml',
}
breathe_default_project = 'flangee77'
breathe_default_members = ('members', 'private-members', 'undoc-members')
