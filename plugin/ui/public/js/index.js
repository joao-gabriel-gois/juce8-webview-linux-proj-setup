// import * as Juce from './juce/index.js';

log('test!');
window.log = log;

const btn = document.createElement('button');
// p.innerText = `${window.__JUCE__.backend}`;
btn.innerText = "Test";

// btn.addEventListener('click', () => log("Click"));
document.body.appendChild(btn);
btn.addEventListener('click', e => {
  setTimeout(() => e.target.style.backgroundColor = "red", 0);
  setTimeout(() => e.target.style.backgroundColor = "white", 2000);
  log('\n\t\tjs element click,', 'js log');
});

function log(...args) {
  const message = args.join(' ');
  const encodedMessage = encodeURIComponent(message);
  window.location.href = `external://log/${encodedMessage}`;
}
