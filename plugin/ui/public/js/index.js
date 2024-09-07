// Uncommenting line bellow is breaking the whole js script
import { getNativeFunction } from './juce/javascript/index.js';


const nativeFunction = getNativeFunction('nativeFunction');
const log = getNativeFunction('log');
// const log = log2;

log('[index.js] Starting JS Script!', 'Running...');
window.log = log;

const btn = document.createElement('button');
btn.innerText = "Test";
btn.addEventListener('click', e => {
  setTimeout(() => e.target.style.backgroundColor = "red", 0);
  setTimeout(() => e.target.style.backgroundColor = "white", 2000);
  log('[index.js]\t js element click,', 'js log2');
});
document.body.appendChild(btn);

// Main
document.addEventListener('DOMContentLoaded', () => {
  const {
    vendor,
    pluginName,
    pluginVersion
  } = window.__JUCE__.initialisationData;

  const $id = (id) => document.getElementById(id);
  $id('vendor').innerText = vendor;
  $id('pluginName').innerText = pluginName;
  $id('pluginVersion').innerText = pluginVersion;

  const fnButton = $id('nativeFnButton');
  fnButton.addEventListener('click', () => {
    nativeFunction("one ", 2, null).then(completionResult => {
      log(completionResult);
    });
  });

  const evButton = $id('emitEvButton');
  let emitCounter = 0;
  evButton.addEventListener('click', () => {
    emitCounter++;
    window.__JUCE__.backend.emitEvent('exampleInnerEvent2', {
      emitCounter,
    });
  });

});

window.__JUCE__.backend.addEventListener("exampleEvent", (objFromCppBackend) => {
  log("[backend-event from emitEventIfBrowserIsVisible]\n\t",
    "\"exampleEvent\" emited from JUCE Backend, getting value =", 
    objFromCppBackend);
});

window.addEventListener('exampleInnerEvent', () => {
  log("[client-event from js eval in cpp]\n\t\"exampleInnerEvent\"",
    "dispatched in client side from cpp through js eval!");
});


// function log2(...args) {
//   const message = args.join(' ');
//   const encodedMessage = encodeURIComponent(message);
//   window.location.href = `external://log/${encodedMessage}`;
// }
