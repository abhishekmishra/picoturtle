(function () {
  const canvas = document.getElementById("turtle-art");
  if (!canvas) return;

  const context = canvas.getContext("2d");
  const colours = ["#d9ff57", "#ff6c56", "#4a72ff", "#f2efe7"];
  context.fillStyle = "#181916";
  context.fillRect(0, 0, canvas.width, canvas.height);
  context.translate(canvas.width / 2, canvas.height / 2);
  context.lineCap = "round";

  let x = 0;
  let y = 0;
  let heading = -Math.PI / 2;

  for (let step = 0; step < 112; step += 1) {
    const distance = 4 + step * 1.75;
    const nextX = x + Math.cos(heading) * distance;
    const nextY = y + Math.sin(heading) * distance;
    context.beginPath();
    context.moveTo(x, y);
    context.lineTo(nextX, nextY);
    context.strokeStyle = colours[Math.floor(step / 7) % colours.length];
    context.lineWidth = step % 7 === 0 ? 4 : 2;
    context.globalAlpha = 0.94;
    context.stroke();
    x = nextX;
    y = nextY;
    heading += (Math.PI / 180) * 91;
  }
})();
