// Service Worker — 只缓存CDN资源，HTML从网络获取
var CACHE = 'sgms-v6';
var CDN_URLS = [
  'https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/highlight.min.js',
  'https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/languages/cpp.min.js',
  'https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/styles/github.min.css',
  'https://cdn.jsdelivr.net/npm/echarts@5.5.0/dist/echarts.min.js'
];

self.addEventListener('install', function(e) {
  e.waitUntil(
    caches.open(CACHE).then(function(c) {
      return Promise.allSettled(CDN_URLS.map(function(u) {
        return c.add(u).catch(function() {});
      }));
    })
  );
});

// HTML走网络，CDN走缓存
self.addEventListener('fetch', function(e) {
  var url = e.request.url;
  if (url.indexOf('cdnjs') >= 0 || url.indexOf('jsdelivr') >= 0) {
    e.respondWith(
      caches.match(e.request).then(function(r) {
        return r || fetch(e.request);
      })
    );
  }
  // HTML等不缓存，直接走网络
});
