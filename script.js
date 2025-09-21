function readMore(btn) {
  alert("This will take you to the full blog post!");
}

// Contact form submission
document.addEventListener("DOMContentLoaded", () => {
  const form = document.getElementById("contactForm");
  const response = document.getElementById("formResponse");

  if (form) {
    form.addEventListener("submit", function(e) {
      e.preventDefault();
      response.textContent = "✅ Thank you! Your message has been sent.";
      form.reset();
    });
  }
});
