
(() => {
    const originalInitNavTree = window.initNavTree;

    window.initNavTree = function(toroot, relpath) {
        originalInitNavTree(toroot, relpath);

        scheduleInitCustomNavTree();
    };

    function scheduleInitCustomNavTree() {
        setTimeout(initCustomNavTree, 25);
    }

    function initCustomNavTree() {
        let allOkay = true;
        allOkay &= tryEnsureCustomItemBefore('li:first-child', 'General');
        allOkay &= tryEnsureCustomItemBefore('li:nth-last-child(4)', 'API Reference');
        if (!allOkay) {
            scheduleInitCustomNavTree();
        }
    }

    function tryEnsureCustomItemBefore(selector, label) {
        const $parent = $('#side-nav #nav-tree #nav-tree-contents > ul > li > ul.children_ul');
        const $element = $parent.find('> ' + selector);

        if ($element.length === 0) {
            return false;
        }

        const idSuffix = label
            .toLowerCase()
            .trim()
            .replace(/[^a-z0-9\s-]+/g, '')
            .replace(/\s+/g, '-')
            .replace(/-+/g,'-')
        ;
        const id = 'custom-item-' + idSuffix;

        if ($element.prev('#' + id).length > 0) {
            return true;
        }

        const $customItem = $('<div/>', {'id': id, 'class': 'custom-item'});
        $customItem.text(label);
        $element.prepend($customItem);
        return true;
    }
})();
