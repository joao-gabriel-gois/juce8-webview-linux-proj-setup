// import * as Juce from './juce/index.js';

log('test!');
window.log = log;

const p = document.createElement('p');
// p.innerText = `${window.__JUCE__.backend}`;
p.innerText = "Test";

// p.addEventListener('click', () => log("Click"));
document.body.appendChild(p);
p.addEventListener('click', e => {
  setTimeout(() => e.target.style.backgroundColor = "red", 0);
  setTimeout(() => e.target.style.backgroundColor = "white", 2000);
  log('click', e.target.tagName);
});

function log(argument) {
  // Encode the argument if necessary and invoke the external log event
  const encodedMessage = encodeURIComponent(argument);
  window.location.href = `external://log/${encodedMessage}`;
}
