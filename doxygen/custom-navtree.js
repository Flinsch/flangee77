
(() => {
    const originalInitNavTree = window.initNavTree;

    window.initNavTree = function(toroot, relpath) {
        originalInitNavTree(toroot, relpath);

        setTimeout(initCustomNavTree, 25);
    };

    function initCustomNavTree() {
        $('#side-nav #nav-tree #nav-tree-contents > ul > li > ul.children_ul .custom-item').remove();
        $('#side-nav #nav-tree #nav-tree-contents > ul > li > ul.children_ul > li:first-child').prepend(createCustomItem('General'));
        $('#side-nav #nav-tree #nav-tree-contents > ul > li > ul.children_ul > li:nth-last-child(4)').prepend(createCustomItem('API Reference'));
    }

    function createCustomItem(label) {
        const $customItem = $('<div/>', {'class': 'custom-item'});
        $customItem.text(label);
        return $customItem;
    }
})();
