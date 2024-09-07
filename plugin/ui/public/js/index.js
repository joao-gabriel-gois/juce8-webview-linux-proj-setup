// Uncommenting line bellow is breaking the whole js script
// import * as Juce from './juce/index.js';

log('test!');
window.log = log;

const btn = document.createElement('button');
btn.innerText = "Test";
btn.addEventListener('click', e => {
  setTimeout(() => e.target.style.backgroundColor = "red", 0);
  setTimeout(() => e.target.style.backgroundColor = "white", 2000);
  log('\t\tjs element click,', 'js log\n');
});
document.body.appendChild(btn);

window.__JUCE__.backend.addEventListener("exampleEvent", (objFromCppBackend) => {
  log("Dispacthing \"exampleEvent\" from C++ with value coming from backend =", objFromCppBackend);
});

window.addEventListener('exampleInnerEvent', () => {
  log("Receiving dispatched \"exampleInnerEvent\" from cpp side js eval!");
});

const {
  vendor,
  pluginName,
  pluginVersion
} = window.__JUCE__.initialisationData;

const $id = (id) => document.getElementById(id);
$id('vendor').innerText = vendor;
$id('pluginName').innerText = pluginName;
$id('pluginVersion').innerText = pluginVersion;


function log(...args) {
  const message = args.join(' ');
  const encodedMessage = encodeURIComponent(message);
  window.location.href = `external://log/${encodedMessage}`;
}

// function log2(...args) {
//   const message = args.join(' ');
  
// }